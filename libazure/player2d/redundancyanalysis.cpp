#include "redundancyanalysis.h"
#include "ui_redundancyanalysis.h"

#include "RecordedEvent.h"
#include <qmessagebox.h>

#include "timer.h"

using namespace mozilla;
using namespace mozilla::gfx;

RedundancyAnalysis::RedundancyAnalysis(PlaybackManager *aPBManager, QWidget *parent) :
    QMainWindow(parent),
    mPBManager(aPBManager),
    ui(new Ui::RedundancyAnalysis)
{
  ui->setupUi(this);
}

RedundancyAnalysis::~RedundancyAnalysis()
{
  delete ui;
}

void RedundancyAnalysis::on_lineEdit_textChanged(const QString &aNewStart)
{
  ui->dtList->clear();
  bool ok;
  uint32_t start = aNewStart.toInt(&ok);

  if (!ok) {
    return;
  }

  mPBManager->PlaybackToEvent(start);

  PlaybackManager::DTMap::iterator iter = mPBManager->mDrawTargets.begin();

  for (;mPBManager->mDrawTargets.end() != iter; iter++) {
    QVariant var((qulonglong)iter->first);
    ui->dtList->addItem(QString::fromStdString(StringFromPtr(iter->first)), var);
  }
}

static bool
CompareSurfaces(DataSourceSurface *aSurfA, DataSourceSurface *aSurfB)
{
  for (int y = 0; y < aSurfA->GetSize().height; y++) {
    if (memcmp(aSurfA->GetData() + y * aSurfA->Stride(), aSurfB->GetData() + y * aSurfB->Stride(), aSurfA->GetSize().width * 4) != 0) {
      return false;
    }
  }
  return true;
}

void RedundancyAnalysis::on_pushButton_clicked()
{
  bool ok;
  uint32_t start = ui->lineEdit->text().toInt(&ok);

  if (!ok) {
    QMessageBox::critical(this, "Error", "Invalid starting event");
    return;
  }

  uint32_t end = ui->lineEdit_2->text().toInt(&ok);
  if (!ok) {
    QMessageBox::critical(this, "Error", "Invalid end event");
    return;
  }

  if (end <= start) {
    QMessageBox::critical(this, "Error", "End event must be after start event");
    return;
  }

  ReferencePtr dtPtr = (void*)(ui->dtList->itemData(ui->dtList->currentIndex()).toUInt());

  mPBManager->PlaybackToEvent(start);
  RefPtr<DrawTarget> dt = mPBManager->LookupDrawTarget(dtPtr);
  QString str = "Beginning Analysis of DT 0x";
  str.append(QString::fromStdString(StringFromPtr(dtPtr)));
  ui->plainTextEdit->appendPlainText(str);

  mPBManager->PlaybackToEvent(end);

  RefPtr<DrawTarget> endDT = mPBManager->LookupDrawTarget(dtPtr);
  if (dt != endDT) {
    ui->plainTextEdit->appendPlainText("Analysis failed");
    return;
  }

#ifdef WIN32
  LARGE_INTEGER startStamp, endStamp, freq;
#endif

  RefPtr<SourceSurface> surf = dt->Snapshot();
  RefPtr<DataSourceSurface> refDataSurf = surf->GetDataSurface();
  ReferencePtr data = refDataSurf->GetData();

  uint32_t redundantEvents = 0;
  ui->progressBar->setEnabled(true);
  ui->progressBar->setMaximum(end - start);
  ui->progressBar->setValue(0);
  for (uint32_t i = start; i < end; i++) {
    mPBManager->PlaybackToEvent(start);
    ui->progressBar->setValue(i);
    QApplication::processEvents();
    mPBManager->DisableEvent(i);
    mPBManager->PlaybackToEvent(end);
    RefPtr<DrawTarget> newDT = mPBManager->LookupDrawTarget(dtPtr);
    RefPtr<SourceSurface> srcSurf = newDT->Snapshot();
    RefPtr<DataSourceSurface> cmpSurf = srcSurf->GetDataSurface();
    if (!CompareSurfaces(cmpSurf, refDataSurf)) {
      mPBManager->EnableEvent(i);
      continue;
    }
    redundantEvents++;
  }

  ui->plainTextEdit->appendPlainText("Analysis finished.");

  str = "Analyzed events: " + QString::number(end - start);
  ui->plainTextEdit->appendPlainText(str);
  str = "Redundant events: " + QString::number(redundantEvents);
  ui->plainTextEdit->appendPlainText(str);

  HighPrecisionMeasurement timer;

  mPBManager->PlaybackToEvent(start);
  dt = mPBManager->LookupDrawTarget(dtPtr);
  dt->Flush();
  
  timer.Start();
  mPBManager->PlaybackToEvent(end);
  dt->Flush();

  double timeSecond = timer.Measure();

  mPBManager->EnableAllEvents();
  mPBManager->PlaybackToEvent(start);
  dt = mPBManager->LookupDrawTarget(dtPtr);
  dt->Flush();

  timer.Start();
  mPBManager->PlaybackToEvent(end);
  dt->Flush();

  double timeFirst = timer.Measure();

  str = "Old timing: " + QString::number(timeFirst, 'g', 3) + " ms";
  ui->plainTextEdit->appendPlainText(str);
  str = "New timing: " + QString::number(timeSecond, 'g', 3) + " ms";
  ui->plainTextEdit->appendPlainText(str);
}

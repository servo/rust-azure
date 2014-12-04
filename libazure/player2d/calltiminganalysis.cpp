#include "calltiminganalysis.h"
#include "ui_calltiminganalysis.h"

#include <qmessagebox.h>

#include "mainwindow.h"

#include "RecordedEvent.h"

using namespace mozilla;
using namespace mozilla::gfx;

CallTimingAnalysis::CallTimingAnalysis(MainWindow *aMainWindow) :
  QMainWindow(aMainWindow),
  ui(new Ui::CallTimingAnalysis),
  mMainWindow(aMainWindow)
{
  setAttribute(Qt::WA_DeleteOnClose);
  ui->setupUi(this);
}

CallTimingAnalysis::~CallTimingAnalysis()
{
  delete ui;
}

void CallTimingAnalysis::on_pushButton_clicked()
{
  bool ok;
  uint32_t start = ui->startCall->text().toInt(&ok);

  if (!ok) {
    QMessageBox::critical(this, "Error", "Invalid starting event");
    return;
  }

  uint32_t end = ui->endCall->text().toInt(&ok);
  if (!ok) {
    QMessageBox::critical(this, "Error", "Invalid end event");
    return;
  }

  if (end <= start) {
    QMessageBox::critical(this, "Error", "End event must be after start event");
    return;
  }
  if (start >= mMainWindow->mEventItems.size() ||
      end >= mMainWindow->mEventItems.size()) {
    QMessageBox::critical(this, "Error", "Start call or end call out of range");
    return;
  }
  ui->progressBar->setEnabled(true);
  ui->progressBar->setMaximum(end - start);
  ui->progressBar->setValue(0);

  ui->resultBox->appendPlainText("Starting analysis...");

  double total = 0;

  double totals[RecordedEvent::kTotalEventTypes];
  for (uint32_t i = 0; i < RecordedEvent::kTotalEventTypes; i++) {
    totals[i] = 0;
  }
  for (uint32_t i = start; i <= end; i++) {
    double stdDev;
    EventItem *item = static_cast<EventItem*>(mMainWindow->mEventItems[i]);
    double avg = mMainWindow->mPBManager.GetEventTiming(item->mID,
                                                        ui->preventBatching->checkState() != Qt::Checked,
                                                        ui->ignoreFirst->checkState() == Qt::Checked,
                                                        ui->forceFlush->checkState() == Qt::Checked,
                                                        ui->forceRealization->checkState() == Qt::Checked, &stdDev);
    
    item->mTiming = avg;
    mMainWindow->mEventItems[i]->setText(3, QString::number(avg, 'g', 3) + " +/- " + QString::number(stdDev, 'g', 2) + " ms");

    total += avg;
    ui->progressBar->setValue(i - start);
    QApplication::processEvents();

    totals[mMainWindow->mPBManager.mRecordedEvents[item->mID]->GetType()] += avg;
  }

  ui->resultBox->appendPlainText("Total time measured: " + QString::number(total, 'g', 3) + " ms");
  for (uint32_t i = 0; i < RecordedEvent::kTotalEventTypes; i++) {
    ui->resultBox->appendPlainText(QString::fromStdString(RecordedEvent::GetEventName((RecordedEvent::EventType)i)) +
                                   ": " + QString::number(totals[i], 'g', 3));
  }
}

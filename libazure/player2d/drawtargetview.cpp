#include "drawtargetview.h"
#include "ui_drawtargetview.h"
#include "2D.h"
#include "RecordedEvent.h"
#include "mainwindow.h"

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

DrawTargetView::DrawTargetView(ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator, QWidget *parent) :
    SurfaceView(parent),
    ui(new Ui::DrawTargetView),
    mRefPtr(aRefPtr),
    mTranslator(aTranslator),
    mClipVisualized(false)
{
    ui->setupUi(this);
    ui->dtWidget->SwitchToBackend(int(MainWindow::mMainBackend));

    connect(ui->dtWidget, SIGNAL(RefillDT()), SLOT(UpdateView()));
    connect(this, SIGNAL(SwitchingBackend(uint32_t)), ui->dtWidget, SLOT(SwitchToBackend(uint32_t)));
    EventChanged();
}

DrawTargetView::~DrawTargetView()
{
    delete ui;
}

TemporaryRef<SourceSurface>
DrawTargetView::GetSourceSurface()
{
  DrawTarget *srcDT = mTranslator->LookupDrawTarget(mRefPtr);

  if (!srcDT) {
    return nullptr;
  }

  RefPtr<SourceSurface> oldSurf;
  if (mClipVisualized) {
    oldSurf = srcDT->Snapshot();
    Matrix mat = srcDT->GetTransform();
    srcDT->SetTransform(Matrix());
    srcDT->FillRect(Rect(0, 0, srcDT->GetSize().width, srcDT->GetSize().height), ColorPattern(Color(1.0f, 0, 0, 1.0f)));
    srcDT->SetTransform(mat);
  }

  RefPtr<SourceSurface> surf = srcDT->Snapshot();

  if (mClipVisualized) {
    srcDT->CopySurface(oldSurf, IntRect(0, 0, srcDT->GetSize().width, srcDT->GetSize().height), IntPoint());
  }
  return surf;
}

DrawTargetWidget*
DrawTargetView::GetDestDTWidget() const
{
  return ui->dtWidget;
}

QScrollBar*
DrawTargetView::GetHorizontalScrollBar() const
{
  return ui->horizontalScrollBar;
}

QScrollBar*
DrawTargetView::GetVerticalScrollBar() const
{
  return ui->verticalScrollBar;
}

void DrawTargetView::on_actionVisualize_Clip_triggered(bool checked)
{
  mClipVisualized = checked;
  UpdateView();
}

void
DrawTargetView::EventChanged()
{
  DrawTarget *srcDT = mTranslator->LookupDrawTarget(mRefPtr);

  Matrix transform;
  if (srcDT) {
    transform = srcDT->GetTransform();
  }

  ui->l11->setText(QString::number(transform._11));
  ui->l12->setText(QString::number(transform._12));
  ui->l21->setText(QString::number(transform._21));
  ui->l22->setText(QString::number(transform._22));
  ui->l31->setText(QString::number(transform._31));
  ui->l32->setText(QString::number(transform._32));
}

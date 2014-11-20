#include "sourcesurfaceview.h"
#include "ui_sourcesurfaceview.h"
#include "2D.h"
#include "RecordedEvent.h"
#include "mainwindow.h"

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

SourceSurfaceView::SourceSurfaceView(ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator, QWidget *parent) :
    SurfaceView(parent),
    ui(new Ui::SourceSurfaceView),
    mRefPtr(aRefPtr),
    mTranslator(aTranslator)
{
    ui->setupUi(this);
    ui->dtWidget->SwitchToBackend(int(MainWindow::mMainBackend));

    connect(ui->dtWidget, SIGNAL(RefillDT()), SLOT(UpdateView()));
    connect(this, SIGNAL(SwitchingBackend(uint32_t)), ui->dtWidget, SLOT(SwitchToBackend(uint32_t)));
}

SourceSurfaceView::~SourceSurfaceView()
{
    delete ui;
}

TemporaryRef<SourceSurface>
SourceSurfaceView::GetSourceSurface()
{
  RefPtr<SourceSurface> surf = mTranslator->LookupSourceSurface(mRefPtr);
  return surf;
}

DrawTargetWidget*
SourceSurfaceView::GetDestDTWidget() const
{
  return ui->dtWidget;
}

QScrollBar*
SourceSurfaceView::GetHorizontalScrollBar() const
{
  return ui->horizontalScrollBar;
}

QScrollBar*
SourceSurfaceView::GetVerticalScrollBar() const
{
  return ui->verticalScrollBar;
}

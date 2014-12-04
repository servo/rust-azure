#include "surfaceview.h"
#include "2D.h"
#include <algorithm>

#include "drawtargetwidget.h"
#include "RecordedEvent.h"
#include <qscrollbar.h>

using namespace mozilla;
using namespace mozilla::gfx;
using namespace std;

const float kZoomSizes[] = { 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f, 16.0f };

SurfaceView::SurfaceView(QWidget *parent) :
    QWidget(parent),
    mZoomFactor(1.0f)
{
}

void
SurfaceView::UpdateView()
{
  DrawTarget *dt = GetDestDTWidget()->GetDT();

  RefPtr<SourceSurface> srcSurf = GetSourceSurface();

  dt->FillRect(Rect(0, 0, 100000, 100000), ColorPattern(Color(0.5f, 0.5f, 0.5f, 1.0f)));

  if (!srcSurf) {
    dt->Flush();
    GetDestDTWidget()->refresh();
    return;
  }

  srcSurf = srcSurf->GetDataSurface();

  IntSize dtSize = dt->GetSize();
  IntSize dstSurfSize = srcSurf->GetSize();
  IntSize dstSize = srcSurf->GetSize();
  dstSize.width = int(float(dstSize.width * mZoomFactor));
  dstSize.height = int(float(dstSize.height * mZoomFactor));

  QScrollBar *horizontalScrollBar = GetHorizontalScrollBar();
  QScrollBar *verticalScrollBar = GetVerticalScrollBar();
  horizontalScrollBar->setPageStep(dtSize.width);
  verticalScrollBar->setPageStep(dtSize.height);
  horizontalScrollBar->setMaximum(max(dstSize.width - dtSize.width, 0));
  verticalScrollBar->setMaximum(max(dstSize.height - dtSize.height, 0));
  int xtranslate = horizontalScrollBar->value();
  int ytranslate = verticalScrollBar->value();
  Matrix mat;
  mat.Translate(-xtranslate, -ytranslate);
  dt->SetTransform(mat);

  RefPtr<DrawTarget> tmpdt = dt->CreateSimilarDrawTarget(IntSize(20, 20), SurfaceFormat::B8G8R8X8);
  tmpdt->FillRect(Rect(0, 0, 10, 10), ColorPattern(Color(1.0f, 1.0f, 1.0f)));
  tmpdt->FillRect(Rect(10, 10, 10, 10), ColorPattern(Color(1.0f, 1.0f, 1.0f)));
  tmpdt->FillRect(Rect(10, 0, 10, 10), ColorPattern(Color(0.7f, 0.7f, 0.7f)));
  tmpdt->FillRect(Rect(0, 10, 10, 10), ColorPattern(Color(0.7f, 0.7f, 0.7f)));
  RefPtr<SourceSurface> src = tmpdt->Snapshot();
  tmpdt = NULL;

  Rect surfRect(0, 0, dstSurfSize.width, dstSurfSize.height);
  Rect dstRect(0, 0, dstSize.width, dstSize.height);
  dt->FillRect(dstRect, SurfacePattern(src, ExtendMode::REPEAT));

  if (srcSurf->GetFormat() != SurfaceFormat::A8) {
    dt->DrawSurface(srcSurf, dstRect, surfRect, DrawSurfaceOptions(mZoomFactor > 1.5f ? Filter::POINT : Filter::LINEAR));
  } else {
    dt->PushClipRect(dstRect);
    dt->Mask(ColorPattern(Color(0, 0, 0, 1)), SurfacePattern(srcSurf, ExtendMode::CLAMP));
    dt->PopClip();
  }

  dt->Flush();

  GetDestDTWidget()->refresh();
}

void SurfaceView::on_actionOriginal_Size_triggered()
{
  mZoomFactor = 1.0f;
  UpdateView();
}

void SurfaceView::on_actionFit_To_Window_triggered()
{
  RefPtr<SourceSurface> srcSurf = GetSourceSurface();

  if (!srcSurf) {
    mZoomFactor = 1.0f;
    return;
  }

  IntSize srcSize = srcSurf->GetSize();
  IntSize dstSize = GetDestDTWidget()->GetDT()->GetSize();

  mZoomFactor = min(float(dstSize.width) / srcSize.width, float(dstSize.height) / srcSize.height);
  UpdateView();
}

void SurfaceView::on_actionZoom_In_triggered()
{
  for (int i = 0; i < sizeof(kZoomSizes) / sizeof(float); i++) {
    if (kZoomSizes[i] > mZoomFactor) {
      mZoomFactor = kZoomSizes[i];
      UpdateView();
      return;
    }
  }

  mZoomFactor = 16.0f;
  UpdateView();
}

void SurfaceView::on_actionZoom_Out_triggered()
{
  for (int i = (sizeof(kZoomSizes) / sizeof(float) - 1); i >= 0; i--) {
    if (kZoomSizes[i] < mZoomFactor) {
      mZoomFactor = kZoomSizes[i];
      UpdateView();
      return;
    }
  }

  mZoomFactor = 0.125f;
  UpdateView();
}

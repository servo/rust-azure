#include "drawtargetwidget.h"

#ifdef WIN32
#include <d3d10_1.h>
#include <dxgi.h>
#endif

#include "qpainter.h"
#include "qevent.h"
#include "mainwindow.h"

using namespace mozilla;
using namespace mozilla::gfx;

DrawTargetWidget::DrawTargetWidget(QWidget *parent) :
    QWidget(parent, Qt::SubWindow),
    mDT(NULL),
    mMainWindow(NULL),
    mType(BackendType::NONE)
{
}

void
DrawTargetWidget::InitDT()
{
  setAttribute(Qt::WA_NoSystemBackground, false);
  setAttribute(Qt::WA_OpaquePaintEvent, false);
  setAttribute(Qt::WA_PaintOnScreen, false); 
#ifdef WIN32
  mSwapChain = nullptr;

  if (mType == BackendType::DIRECT2D) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen); 

    RefPtr<IDXGIDevice> dxgiDevice;
    RefPtr<IDXGIAdapter> dxgiAdapter;

    Factory::GetDirect3D10Device()->QueryInterface((IDXGIDevice**)byRef(dxgiDevice));
    dxgiDevice->GetAdapter(byRef(dxgiAdapter));

    RefPtr<IDXGIFactory> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS((IDXGIFactory**)byRef(dxgiFactory)));

    DXGI_SWAP_CHAIN_DESC swapDesc;
    ::ZeroMemory(&swapDesc, sizeof(swapDesc));
    swapDesc.BufferDesc.Width = 0;
    swapDesc.BufferDesc.Height = 0;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 1;
    swapDesc.OutputWindow = (HWND)winId();
    swapDesc.Windowed = TRUE;

    dxgiFactory->CreateSwapChain(dxgiDevice, &swapDesc, byRef(mSwapChain));

    ID3D10Texture2D *texture;
    mSwapChain->GetBuffer(0, IID_ID3D10Texture2D, (void**)&texture);
    mDT = Factory::CreateDrawTargetForD3D10Texture(texture, SurfaceFormat::B8G8R8X8);
    texture->Release();

    mSwapChain->Present(0, 0);
  } else
#endif
  {
    mDT = Factory::CreateDrawTarget(mType, IntSize(width(),height()), SurfaceFormat::B8G8R8X8);
  }
}

#ifdef WIN32
#if QT_VERSION >= 0x050000
bool
DrawTargetWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
  return winEvent((MSG*)message, result);
}
#endif

bool
DrawTargetWidget::winEvent(MSG* message, long* result)
{
  if (mDT && mDT->GetType() == BackendType::DIRECT2D) {
    if (message->hwnd == (HWND)this->effectiveWinId() && message->message == WM_SIZE) {
      InitDT();
      RefillDT();
    } else if (message->hwnd == (HWND)this->effectiveWinId() && message->message == WM_PAINT) {
      PAINTSTRUCT pt;
      ::BeginPaint(message->hwnd, &pt);
      ::EndPaint(message->hwnd, &pt);
      redraw();
      return true;
    } else if (message->hwnd == (HWND)this->effectiveWinId() && message->message == WM_WINDOWPOSCHANGED) {
      ::InvalidateRect((HWND)effectiveWinId(), NULL, FALSE);
    }
  }

  return false;
}
#endif

void
DrawTargetWidget::refresh()
{
  if (mDT && mDT->GetType() == BackendType::DIRECT2D) {
    redraw();
  } else {
    repaint();
  }
}

void
DrawTargetWidget::redraw()
{
#ifdef WIN32
  if (mSwapChain) {
    mSwapChain->Present(0, 0);
  }
#endif

  if (mDT && mDT->GetType() != BackendType::DIRECT2D) {
    QPainter painter(this);

    RefPtr<SourceSurface> srcSurf = mDT->Snapshot();
    RefPtr<DataSourceSurface> dataSrc = srcSurf->GetDataSurface();
    QImage qimage(dataSrc->GetData(), width(), height(), dataSrc->Stride(), QImage::Format_RGB32);
    painter.drawImage(QRect(0,0,width(), height()), qimage);
  }
}

void
DrawTargetWidget::resizeEvent(QResizeEvent * aEvent)
{
// On windows this is handled by winEvent

  if (mDT && mDT->GetType() != BackendType::DIRECT2D) {
    InitDT();
    RefillDT();
  }
}

void
DrawTargetWidget::paintEvent(QPaintEvent *aEvent)
{
  redraw();
}

void
DrawTargetWidget::SwitchToBackend(uint32_t aType)
{
  mType = BackendType(aType);
  InitDT();
  RefillDT();
}

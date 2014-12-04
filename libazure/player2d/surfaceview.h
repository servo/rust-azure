#ifndef SURFACEVIEW_H
#define SURFACEVIEW_H

#include <QWidget>
#include "2D.h"

#include "RecordedEvent.h"

class DrawTargetWidget;
class QScrollBar;

class SurfaceView : public QWidget
{
    Q_OBJECT

public:
    explicit SurfaceView(QWidget *parent = 0);

protected:
    virtual mozilla::TemporaryRef<mozilla::gfx::SourceSurface> GetSourceSurface() = 0;
    virtual DrawTargetWidget *GetDestDTWidget() const = 0;
    virtual QScrollBar *GetHorizontalScrollBar() const = 0;
    virtual QScrollBar *GetVerticalScrollBar() const = 0;

public slots:
    void UpdateView();

signals:
  void SwitchingBackend(uint32_t aType);

protected slots:
    void on_actionZoom_In_triggered();
    void on_actionOriginal_Size_triggered();
    void on_actionFit_To_Window_triggered();
    void on_actionZoom_Out_triggered();

private:
    float mZoomFactor;
};


#endif // SURFACEVIEW_H

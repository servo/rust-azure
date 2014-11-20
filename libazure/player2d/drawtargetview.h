#ifndef DRAWTARGETVIEW_H
#define DRAWTARGETVIEW_H

#include "surfaceview.h"

namespace Ui {
class DrawTargetView;
}

namespace mozilla {
namespace gfx {
class Translator;
}
}

class DrawTargetView : public SurfaceView
{
    Q_OBJECT
    
public:
    explicit DrawTargetView(mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator,
                            QWidget *parent = 0);
    ~DrawTargetView();

    virtual mozilla::TemporaryRef<mozilla::gfx::SourceSurface> GetSourceSurface();
    virtual DrawTargetWidget *GetDestDTWidget() const;
    virtual QScrollBar *GetHorizontalScrollBar() const;
    virtual QScrollBar *GetVerticalScrollBar() const;

protected slots:
    void on_actionVisualize_Clip_triggered(bool aChecked);
    void EventChanged();

private:
    Ui::DrawTargetView *ui;

    mozilla::gfx::ReferencePtr mRefPtr;
    mozilla::gfx::Translator *mTranslator;
    bool mClipVisualized;
};

#endif // DRAWTARGETVIEW_H

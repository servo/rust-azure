#ifndef SOURCESURFACEVIEW_H
#define SOURCESURFACEVIEW_H

#include "surfaceview.h"

namespace Ui {
class SourceSurfaceView;
}

namespace mozilla {
namespace gfx {
class Translator;
}
}

class SourceSurfaceView : public SurfaceView
{
    Q_OBJECT
    
public:
    explicit SourceSurfaceView(mozilla::gfx::ReferencePtr aRefPtr, mozilla::gfx::Translator *aTranslator,
                               QWidget *parent = 0);
    ~SourceSurfaceView();

    virtual mozilla::TemporaryRef<mozilla::gfx::SourceSurface> GetSourceSurface();
    virtual DrawTargetWidget *GetDestDTWidget() const;
    virtual QScrollBar *GetHorizontalScrollBar() const;
    virtual QScrollBar *GetVerticalScrollBar() const;
private:
    Ui::SourceSurfaceView *ui;

    mozilla::gfx::ReferencePtr mRefPtr;
    mozilla::gfx::Translator *mTranslator;
};

#endif // SOURCESURFACEVIEW_H

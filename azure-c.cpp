#include "azure-c.h"
#include "mozilla/gfx/2D.h"

#include <assert.h>

#ifdef USE_CAIRO
#include <cairo/cairo.h>
#endif

using namespace mozilla;

extern "C"
void AzureCSanityCheck() {
    assert(sizeof(Float) == sizeof(gfx::Float));
    assert(sizeof(Color) == sizeof(gfx::Color));
    assert(sizeof(GradientStop) == sizeof(gfx::GradientStop));
    assert(sizeof(Rect) == sizeof(gfx::Rect));
    assert(sizeof(IntRect) == sizeof(gfx::IntRect));
}

#ifdef USE_CAIRO
extern "C"
DrawTargetRef CreateDrawTargetForCairoSurface(cairo_surface_t* aSurface) {
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateDrawTargetForCairoSurface(aSurface);
    target->AddRef();
    return static_cast<DrawTargetRef>(target);
}
#endif

extern "C"
void ReleaseDrawTarget(DrawTargetRef aTarget) {
    gfx::DrawTarget *target = static_cast<gfx::DrawTarget*>(aTarget);
    target->Release();
}

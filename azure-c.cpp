#include "azure-c.h"
#include "mozilla/gfx/2D.h"

#include <assert.h>

using namespace mozilla;

extern "C"
void AzSanityCheck() {
    assert(sizeof(AzFloat) == sizeof(gfx::Float));
    assert(sizeof(AzColor) == sizeof(gfx::Color));
    assert(sizeof(AzGradientStop) == sizeof(gfx::GradientStop));
    assert(sizeof(AzRect) == sizeof(gfx::Rect));
    assert(sizeof(AzIntRect) == sizeof(gfx::IntRect));
    assert(sizeof(AzPoint) == sizeof(gfx::Point));
    assert(sizeof(AzIntPoint) == sizeof(gfx::IntPoint));
    assert(sizeof(AzSize) == sizeof(gfx::Size));
    assert(sizeof(AzIntSize) == sizeof(gfx::IntSize));
}

#ifdef USE_CAIRO
extern "C"
AzDrawTargetRef AzCreateDrawTargetForCairoSurface(cairo_surface_t* aSurface) {
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateDrawTargetForCairoSurface(aSurface);
    target->AddRef();
    return target;
}
#endif

extern "C"
void AzReleaseDrawTarget(AzDrawTargetRef aTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aTarget);
    gfxDrawTarget->Release();
}

extern "C"
AzColorPatternRef AzCreateColorPattern(AzColor *aColor) {
    gfx::Color *gfxColor = reinterpret_cast<gfx::Color*>(aColor);
    gfx::ColorPattern *gfxColorPattern = new gfx::ColorPattern(*gfxColor);
    return gfxColorPattern;
}

extern "C"
void AzReleaseColorPattern(AzColorPatternRef aColorPattern) {
    gfx::ColorPattern *gfxColorPattern = static_cast<gfx::ColorPattern*>(aColorPattern);
    delete gfxColorPattern;
}

extern "C"
void AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget, AzRect *aRect, AzPatternRef aPattern) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfxDrawTarget->FillRect(*gfxRect, *gfxPattern);
}

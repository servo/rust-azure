#include "azure-c.h"
#include "mozilla/gfx/2D.h"

#include <assert.h>

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
    return target;
}
#endif

extern "C"
void ReleaseDrawTarget(DrawTargetRef aTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aTarget);
    gfxDrawTarget->Release();
}

extern "C"
ColorPatternRef CreateColorPattern(Color *aColor) {
    gfx::Color *gfxColor = reinterpret_cast<gfx::Color*>(aColor);
    return new gfx::ColorPattern(*gfxColor);
}

extern "C"
void ReleaseColorPattern(ColorPatternRef aColorPattern) {
    gfx::Color *gfxColorPattern = static_cast<gfx::Color*>(aColorPattern);
    delete gfxColorPattern;
}

extern "C"
void DrawTargetFillRect(DrawTargetRef aDrawTarget, Rect *aRect, PatternRef aPattern) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfxDrawTarget->FillRect(*gfxRect, *gfxPattern);
}

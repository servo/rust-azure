/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "azure-c.h"
#include "2D.h"
#include "Filters.h"
#include "ScaledFontSkia.h"
#include "Types.h"

#include <assert.h>
#include <string.h>

using namespace mozilla;

// Utilities

static AzIntSize IntSizeToC(gfx::IntSize src) {
    AzIntSize dst;
    memcpy(&dst, &src, sizeof(dst));
    return dst;
}


#define CHECK_SIZE(name) assert(sizeof(Az##name) == sizeof(gfx::name))
#define CHECK_ENUM(azureEnumName, rustAzurePrefix, name) assert((int)rustAzurePrefix##_##name == (int)gfx::azureEnumName::name)

#define STATIC_ASSERT_EQUALS(a, b)\
    typedef char assert_failed_ ## name [ (a - b) ? 1 : -1 ]

extern "C"
void AzSanityCheck() {
    CHECK_SIZE(Matrix);
    CHECK_SIZE(Float);
    CHECK_SIZE(Color);
    CHECK_SIZE(GradientStop);
    CHECK_SIZE(Rect);
    CHECK_SIZE(IntRect);
    CHECK_SIZE(Point);
    CHECK_SIZE(IntPoint);
    CHECK_SIZE(Size);
    CHECK_SIZE(IntSize);
    CHECK_SIZE(DrawOptions);
    CHECK_SIZE(StrokeOptions);
    CHECK_SIZE(DrawSurfaceOptions);
    CHECK_SIZE(Glyph);
    CHECK_SIZE(GlyphBuffer);
    CHECK_SIZE(NativeFont);

    CHECK_ENUM(SurfaceType, AZ_SURFACE, DATA);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, D2D1_BITMAP);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, D2D1_DRAWTARGET);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, CAIRO);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, CAIRO_IMAGE);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, COREGRAPHICS_IMAGE);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, COREGRAPHICS_CGCONTEXT);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, SKIA);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, DUAL_DT);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, D2D1_1_IMAGE);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, RECORDING);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, NVPR_TEXTURE);
    CHECK_ENUM(SurfaceType, AZ_SURFACE, TILED);

    CHECK_ENUM(SurfaceFormat, AZ_FORMAT, B8G8R8A8);
    CHECK_ENUM(SurfaceFormat, AZ_FORMAT, B8G8R8X8);
    CHECK_ENUM(SurfaceFormat, AZ_FORMAT, R5G6B5);
    CHECK_ENUM(SurfaceFormat, AZ_FORMAT, A8);

    CHECK_ENUM(BackendType, AZ_BACKEND, NONE);
    CHECK_ENUM(BackendType, AZ_BACKEND, DIRECT2D);
    CHECK_ENUM(BackendType, AZ_BACKEND, COREGRAPHICS);
    CHECK_ENUM(BackendType, AZ_BACKEND, CAIRO);
    CHECK_ENUM(BackendType, AZ_BACKEND, SKIA);
    CHECK_ENUM(BackendType, AZ_BACKEND, RECORDING);
    CHECK_ENUM(BackendType, AZ_BACKEND, DIRECT2D1_1);
    CHECK_ENUM(BackendType, AZ_BACKEND, NVPR);

    CHECK_ENUM(FontType, AZ_FONT, DWRITE);
    CHECK_ENUM(FontType, AZ_FONT, GDI);
    CHECK_ENUM(FontType, AZ_FONT, MAC);
    CHECK_ENUM(FontType, AZ_FONT, SKIA);
    CHECK_ENUM(FontType, AZ_FONT, CAIRO);
    CHECK_ENUM(FontType, AZ_FONT, COREGRAPHICS);
    CHECK_ENUM(FontType, AZ_FONT, NVPR);

    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, D3D10_TEXTURE);
    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, CAIRO_SURFACE);
    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, CAIRO_CONTEXT);
    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, CGCONTEXT);
    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, CGCONTEXT_ACCELERATED);
    CHECK_ENUM(NativeSurfaceType, AZ_NATIVE_SURFACE, OPENGL_TEXTURE);

    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, DWRITE_FONT_FACE);
    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, GDI_FONT_FACE);
    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, MAC_FONT_FACE);
    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, SKIA_FONT_FACE);
    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, CAIRO_FONT_FACE);
    CHECK_ENUM(NativeFontType, AZ_NATIVE_FONT, NVPR_FONT_FACE);

    CHECK_ENUM(CompositionOp, AZ, OP_OVER);
    CHECK_ENUM(CompositionOp, AZ, OP_ADD);
    CHECK_ENUM(CompositionOp, AZ, OP_ATOP);
    CHECK_ENUM(CompositionOp, AZ, OP_OUT);
    CHECK_ENUM(CompositionOp, AZ, OP_IN);
    CHECK_ENUM(CompositionOp, AZ, OP_SOURCE);
    CHECK_ENUM(CompositionOp, AZ, OP_DEST_IN);
    CHECK_ENUM(CompositionOp, AZ, OP_DEST_OUT);
    CHECK_ENUM(CompositionOp, AZ, OP_DEST_OVER);
    CHECK_ENUM(CompositionOp, AZ, OP_DEST_ATOP);
    CHECK_ENUM(CompositionOp, AZ, OP_XOR);
    CHECK_ENUM(CompositionOp, AZ, OP_MULTIPLY);
    CHECK_ENUM(CompositionOp, AZ, OP_SCREEN);
    CHECK_ENUM(CompositionOp, AZ, OP_OVERLAY);
    CHECK_ENUM(CompositionOp, AZ, OP_DARKEN);
    CHECK_ENUM(CompositionOp, AZ, OP_LIGHTEN);
    CHECK_ENUM(CompositionOp, AZ, OP_COLOR_DODGE);
    CHECK_ENUM(CompositionOp, AZ, OP_COLOR_BURN);
    CHECK_ENUM(CompositionOp, AZ, OP_HARD_LIGHT);
    CHECK_ENUM(CompositionOp, AZ, OP_SOFT_LIGHT);
    CHECK_ENUM(CompositionOp, AZ, OP_DIFFERENCE);
    CHECK_ENUM(CompositionOp, AZ, OP_EXCLUSION);
    CHECK_ENUM(CompositionOp, AZ, OP_HUE);
    CHECK_ENUM(CompositionOp, AZ, OP_SATURATION);
    CHECK_ENUM(CompositionOp, AZ, OP_COLOR);
    CHECK_ENUM(CompositionOp, AZ, OP_LUMINOSITY);
    CHECK_ENUM(CompositionOp, AZ, OP_COUNT);

    CHECK_ENUM(ExtendMode, AZ_EXTEND, CLAMP);
    CHECK_ENUM(ExtendMode, AZ_EXTEND, REPEAT);
    CHECK_ENUM(ExtendMode, AZ_EXTEND, REFLECT);

    CHECK_ENUM(FillRule, AZ, FILL_WINDING);
    CHECK_ENUM(FillRule, AZ, FILL_EVEN_ODD);

    CHECK_ENUM(AntialiasMode, AZ_AA, NONE);
    CHECK_ENUM(AntialiasMode, AZ_AA, GRAY);
    CHECK_ENUM(AntialiasMode, AZ_AA, SUBPIXEL);

    CHECK_ENUM(Filter, AZ_FILTER, GOOD);
    CHECK_ENUM(Filter, AZ_FILTER, LINEAR);
    CHECK_ENUM(Filter, AZ_FILTER, POINT);

    CHECK_ENUM(PatternType, AZ_PATTERN, COLOR);
    CHECK_ENUM(PatternType, AZ_PATTERN, SURFACE);
    CHECK_ENUM(PatternType, AZ_PATTERN, LINEAR_GRADIENT);
    CHECK_ENUM(PatternType, AZ_PATTERN, RADIAL_GRADIENT);

    CHECK_ENUM(JoinStyle, AZ_JOIN, BEVEL);
    CHECK_ENUM(JoinStyle, AZ_JOIN, ROUND);
    CHECK_ENUM(JoinStyle, AZ_JOIN, MITER);
    CHECK_ENUM(JoinStyle, AZ_JOIN, MITER_OR_BEVEL);

    CHECK_ENUM(CapStyle, AZ_CAP, BUTT);
    CHECK_ENUM(CapStyle, AZ_CAP, ROUND);
    CHECK_ENUM(CapStyle, AZ_CAP, SQUARE);

    CHECK_ENUM(SamplingBounds, AZ_SAMPLING, UNBOUNDED);
    CHECK_ENUM(SamplingBounds, AZ_SAMPLING, BOUNDED);

    assert((int)AZ_eSideTop == (int)mozilla::eSideTop);
    assert((int)AZ_eSideRight == (int)mozilla::eSideRight);
    assert((int)AZ_eSideBottom == (int)mozilla::eSideBottom);
    assert((int)AZ_eSideLeft == (int)mozilla::eSideLeft);
}

extern "C" AzColorPatternRef
AzCreateColorPattern(AzColor *aColor) {
    gfx::Color *gfxColor = reinterpret_cast<gfx::Color*>(aColor);
    gfx::ColorPattern *gfxColorPattern = new gfx::ColorPattern(*gfxColor);
    return gfxColorPattern;
}

extern "C" AzDrawTargetRef
AzCreateDrawTarget(AzBackendType aBackend, AzIntSize *aSize, AzSurfaceFormat aFormat) {
    gfx::BackendType backendType = static_cast<gfx::BackendType>(aBackend);
    gfx::IntSize *size = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat surfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateDrawTarget(backendType,
                                                                    *size,
                                                                    surfaceFormat);
    target->AddRef();
    return target;
}

extern "C" AzDrawTargetRef
AzCreateDrawTargetForData(AzBackendType aBackend, unsigned char *aData, AzIntSize *aSize,
                          int32_t aStride, AzSurfaceFormat aFormat) {
    gfx::BackendType backendType = static_cast<gfx::BackendType>(aBackend);
    gfx::IntSize *size = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat surfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateDrawTargetForData(backendType,
                                                                           aData,
                                                                           *size,
                                                                           aStride,
                                                                           surfaceFormat);
    if (target != NULL) {
        target->AddRef();
    }
    return target;
}

extern "C" AzDrawTargetRef
AzCreateDrawTargetSkiaWithGrContextAndFBO(SkiaGrContextRef aGrContext, unsigned int aFBOID, AzIntSize *aSize, AzSurfaceFormat aFormat) {
    GrContext *grContext = reinterpret_cast<GrContext*>(aGrContext);
    gfx::IntSize *size = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat surfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateDrawTargetSkiaWithGrContextAndFBO(grContext,
                                                                                           aFBOID,
                                                                                           *size,
                                                                                           surfaceFormat);
    if (target != NULL) {
        target->AddRef();
    }
    return target;
}

extern "C" void
AzRetainDrawTarget(AzDrawTargetRef aTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aTarget);
    gfxDrawTarget->AddRef();
}

extern "C" void
AzReleaseDrawTarget(AzDrawTargetRef aTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aTarget);
    gfxDrawTarget->Release();
}

extern "C" AzIntSize
AzDrawTargetGetSize(AzDrawTargetRef aDrawTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    return IntSizeToC(gfxDrawTarget->GetSize());
}

extern "C" AzSurfaceFormat
AzDrawTargetGetFormat(AzDrawTargetRef aDrawTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    return static_cast<AzSurfaceFormat>(static_cast<int>(gfxDrawTarget->GetFormat()));
}

extern "C" void
AzDrawTargetGetTransform(AzDrawTargetRef aDrawTarget, AzMatrix* aOutMatrix) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    *reinterpret_cast<gfx::Matrix*>(aOutMatrix) = gfxDrawTarget->GetTransform();
}

extern "C" void
AzDrawTargetFlush(AzDrawTargetRef aDrawTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfxDrawTarget->Flush();
}

extern "C" void
AzDrawTargetClearRect(AzDrawTargetRef aDrawTarget, AzRect *aRect) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfxDrawTarget->ClearRect(*gfxRect);
}

extern "C" void
AzDrawTargetFill(AzDrawTargetRef aDrawTarget, AzPathRef aPath,
                 AzPatternRef aPattern, AzDrawOptions *aDrawOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Path *gfxPath = static_cast<gfx::Path*>(aPath);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<gfx::DrawOptions*>(aDrawOptions);
    gfxDrawTarget->Fill(gfxPath, *gfxPattern, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetStroke(AzDrawTargetRef aDrawTarget, AzPathRef aPath,
                   AzPatternRef aPattern, const AzStrokeOptions *aStrokeOptions,
                   const AzDrawOptions *aDrawOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    const gfx::Path *gfxPath = reinterpret_cast<const gfx::Path*>(aPath);
    const gfx::Pattern *gfxPattern = reinterpret_cast<const gfx::Pattern*>(aPattern);
    const gfx::StrokeOptions *gfxStrokeOptions = reinterpret_cast<const gfx::StrokeOptions*>(aStrokeOptions);
    const gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<const gfx::DrawOptions*>(aDrawOptions);
    gfxDrawTarget->Stroke(gfxPath, *gfxPattern, *gfxStrokeOptions, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetPushClip(AzDrawTargetRef aDrawTarget, AzPathRef aPath) {
  gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
  const gfx::Path *gfxPath = reinterpret_cast<const gfx::Path*>(aPath);
  gfxDrawTarget->PushClip(gfxPath);
}

extern "C" void
AzDrawTargetPushClipRect(AzDrawTargetRef aDrawTarget, const AzRect *aRect) {
  gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
  const gfx::Rect *gfxRect = reinterpret_cast<const gfx::Rect*>(aRect);
  gfxDrawTarget->PushClipRect(*gfxRect);
}

extern "C" void
AzDrawTargetPopClip(AzDrawTargetRef aDrawTarget) {
  gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
  gfxDrawTarget->PopClip();
}

extern "C" void
AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget,
                     AzRect* aRect,
                     AzPatternRef aPattern,
                     AzDrawOptions* aDrawOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<gfx::DrawOptions*>(aDrawOptions);
    gfxDrawTarget->FillRect(*gfxRect,
                            *gfxPattern,
                            gfxDrawOptions != NULL ? *gfxDrawOptions : gfx::DrawOptions());
}

extern "C" void
AzDrawTargetStrokeRect(AzDrawTargetRef aDrawTarget, AzRect *aRect,
                       AzPatternRef aPattern, AzStrokeOptions *aStrokeOptions,
                       AzDrawOptions *aDrawOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfx::StrokeOptions *gfxStrokeOptions = reinterpret_cast<gfx::StrokeOptions*>(aStrokeOptions);
    gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<gfx::DrawOptions*>(aDrawOptions);
    gfxDrawTarget->StrokeRect(*gfxRect, *gfxPattern, *gfxStrokeOptions, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetStrokeLine(AzDrawTargetRef aDrawTarget,
                       AzPoint *aStart, AzPoint *aEnd,
                       AzPatternRef aPattern,
                       AzStrokeOptions *aStrokeOptions,
                       AzDrawOptions *aDrawOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Point *gfxStart = reinterpret_cast<gfx::Point*>(aStart);
    gfx::Point *gfxEnd = reinterpret_cast<gfx::Point*>(aEnd);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfx::StrokeOptions *gfxStrokeOptions = reinterpret_cast<gfx::StrokeOptions*>(aStrokeOptions);
    gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<gfx::DrawOptions*>(aDrawOptions);
    gfxDrawTarget->StrokeLine(*gfxStart, *gfxEnd, *gfxPattern, *gfxStrokeOptions, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetFillGlyphs(AzDrawTargetRef aDrawTarget,
                       AzScaledFontRef aFont,
                       AzGlyphBuffer *aGlyphBuffer,
                       AzPatternRef aPattern,
                       AzDrawOptions *aOptions,
                       AzGlyphRenderingOptionsRef aRenderingOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::ScaledFont *gfxScaledFont = static_cast<gfx::ScaledFont*>(aFont);
    gfx::GlyphBuffer *gfxGlyphBuffer = reinterpret_cast<gfx::GlyphBuffer*>(aGlyphBuffer);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfx::DrawOptions *gfxOptions = reinterpret_cast<gfx::DrawOptions*>(aOptions);
    gfx::GlyphRenderingOptions *gfxRenderingOptions = static_cast<gfx::GlyphRenderingOptions*>(aRenderingOptions);
    gfxDrawTarget->FillGlyphs(gfxScaledFont, *gfxGlyphBuffer, *gfxPattern, *gfxOptions, gfxRenderingOptions);
}

extern "C" void
AzDrawTargetDrawSurface(AzDrawTargetRef aDrawTarget,
                        AzSourceSurfaceRef aSurface,
                        AzRect *aDest,
                        AzRect *aSource,
                        AzDrawSurfaceOptionsRef aSurfOptions,
                        AzDrawOptions *aOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::SourceSurface *gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    gfx::Rect *gfxDest = reinterpret_cast<gfx::Rect*>(aDest);
    gfx::Rect *gfxSource = reinterpret_cast<gfx::Rect*>(aSource);
    gfx::DrawSurfaceOptions *gfxDrawSurfaceOptions = static_cast<gfx::DrawSurfaceOptions*>(aSurfOptions);
    gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<gfx::DrawOptions*>(aOptions);
    gfxDrawTarget->DrawSurface(gfxSourceSurface, *gfxDest, *gfxSource, *gfxDrawSurfaceOptions, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetDrawFilter(AzDrawTargetRef aDrawTarget,
                       AzFilterNodeRef aFilter,
                       const AzRect *aSourceRect,
                       const AzPoint *aDestPoint,
                       const AzDrawOptions *aOptions) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::FilterNode *gfxFilterNode = static_cast<gfx::FilterNode*>(aFilter);
    const gfx::Rect *gfxSourceRect = reinterpret_cast<const gfx::Rect*>(aSourceRect);
    const gfx::Point *gfxDestPoint = reinterpret_cast<const gfx::Point*>(aDestPoint);
    const gfx::DrawOptions *gfxDrawOptions = reinterpret_cast<const gfx::DrawOptions*>(aOptions);
    gfxDrawTarget->DrawFilter(gfxFilterNode, *gfxSourceRect, *gfxDestPoint, *gfxDrawOptions);
}

extern "C" void
AzDrawTargetDrawSurfaceWithShadow(AzDrawTargetRef aDrawTarget,
                                  AzSourceSurfaceRef aSurface,
                                  const AzPoint* aPoint,
                                  const AzColor* aColor,
                                  const AzPoint* aOffset,
                                  AzFloat aSigma,
                                  AzCompositionOp aOperator) {
    gfx::DrawTarget* gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::SourceSurface* gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    const gfx::Point* gfxPoint = reinterpret_cast<const gfx::Point*>(aPoint);
    const gfx::Color* gfxColor = reinterpret_cast<const gfx::Color*>(aColor);
    const gfx::Point* gfxOffset = reinterpret_cast<const gfx::Point*>(aOffset);
    gfx::CompositionOp gfxOperator = static_cast<gfx::CompositionOp>(aOperator);
    gfxDrawTarget->DrawSurfaceWithShadow(gfxSourceSurface,
                                         *gfxPoint,
                                         *gfxColor,
                                         *gfxOffset,
                                         aSigma,
                                         gfxOperator);
}

extern "C" AzSourceSurfaceRef
AzDrawTargetGetSnapshot(AzDrawTargetRef aDrawTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    RefPtr<gfx::SourceSurface> gfxSourceSurface = gfxDrawTarget->Snapshot();
    gfxSourceSurface->AddRef();
    return gfxSourceSurface;
}

extern "C" AzSourceSurfaceRef
AzDrawTargetCreateSourceSurfaceFromData(AzDrawTargetRef aDrawTarget,
                                        const unsigned char *aData,
                                        AzIntSize *aSize,
                                        int32_t aStride,
                                        AzSurfaceFormat aFormat) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::IntSize *gfxSize = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat gfxSurfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::SourceSurface> gfxSourceSurface =
        gfxDrawTarget->CreateSourceSurfaceFromData(const_cast<unsigned char *>(aData),
                                                   *gfxSize,
                                                   aStride,
                                                   gfxSurfaceFormat);
    gfxSourceSurface->AddRef();
    return gfxSourceSurface;
}

extern "C" AzDrawTargetRef
AzDrawTargetCreateSimilarDrawTarget(AzDrawTargetRef aDrawTarget,
                                    const AzIntSize *aSize,
                                    AzSurfaceFormat aFormat) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    const gfx::IntSize *gfxSize = reinterpret_cast<const gfx::IntSize*>(aSize);
    gfx::SurfaceFormat gfxSurfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> newDrawTarget =
        gfxDrawTarget->CreateSimilarDrawTarget(*gfxSize, gfxSurfaceFormat);
    newDrawTarget->AddRef();
    return newDrawTarget;
}

extern "C" AzDrawTargetRef
AzDrawTargetCreateShadowDrawTarget(AzDrawTargetRef aDrawTarget,
                                   const AzIntSize *aSize,
                                   AzSurfaceFormat aFormat,
                                   AzFloat aSigma) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    const gfx::IntSize *gfxSize = reinterpret_cast<const gfx::IntSize*>(aSize);
    gfx::SurfaceFormat gfxSurfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> newDrawTarget =
        gfxDrawTarget->CreateShadowDrawTarget(*gfxSize, gfxSurfaceFormat, aSigma);
    newDrawTarget->AddRef();
    return newDrawTarget;
}

extern "C" AzGradientStopsRef
AzDrawTargetCreateGradientStops(AzDrawTargetRef aDrawTarget,
                                AzGradientStop *aStops,
                                uint32_t aNumStops,
                                AzExtendMode aExtendMode) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::GradientStop *gfxStops = reinterpret_cast<gfx::GradientStop*>(aStops);
    gfx::ExtendMode gfxExtendMode = static_cast<gfx::ExtendMode>(aExtendMode);
    RefPtr<gfx::GradientStops> gfxGradientStops =
        gfxDrawTarget->CreateGradientStops(gfxStops, aNumStops, gfxExtendMode);
    gfxGradientStops->AddRef();
    return gfxGradientStops;
}

extern "C" void
AzReleaseSourceSurface(AzSourceSurfaceRef aSurface) {
    gfx::SourceSurface *gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    gfxSourceSurface->Release();
}

extern "C" AzIntSize
AzSourceSurfaceGetSize(AzSourceSurfaceRef aSurface) {
    gfx::SourceSurface *gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    return IntSizeToC(gfxSourceSurface->GetSize());
}

extern "C" AzSurfaceFormat
AzSourceSurfaceGetFormat(AzSourceSurfaceRef aSurface) {
    gfx::SourceSurface *gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    switch (gfxSourceSurface->GetFormat()) {
        case gfx::SurfaceFormat::B8G8R8A8:
            return AZ_FORMAT_B8G8R8A8;
        case gfx::SurfaceFormat::B8G8R8X8:
            return AZ_FORMAT_B8G8R8X8;
        case gfx::SurfaceFormat::R8G8B8A8:
            return AZ_FORMAT_R8G8B8A8;
        case gfx::SurfaceFormat::R8G8B8X8:
            return AZ_FORMAT_R8G8B8X8;
        case gfx::SurfaceFormat::R5G6B5:
            return AZ_FORMAT_R5G6B5;
        case gfx::SurfaceFormat::A8:
            return AZ_FORMAT_A8;
        case gfx::SurfaceFormat::YUV:
            return AZ_FORMAT_YUV;
        case gfx::SurfaceFormat::UNKNOWN:
            return AZ_FORMAT_UNKNOWN;
    }
}

extern "C" AzDataSourceSurfaceRef
AzSourceSurfaceGetDataSurface(AzSourceSurfaceRef aSurface) {
    gfx::SourceSurface *gfxSourceSurface = static_cast<gfx::SourceSurface*>(aSurface);
    RefPtr<gfx::DataSourceSurface> gfxDataSourceSurface = gfxSourceSurface->GetDataSurface();
    gfxDataSourceSurface->AddRef();
    return gfxDataSourceSurface;
}

extern "C" uint8_t *AzDataSourceSurfaceGetData(AzDataSourceSurfaceRef aSurface) {
    gfx::DataSourceSurface *gfxDataSourceSurface = static_cast<gfx::DataSourceSurface*>(aSurface);
    return gfxDataSourceSurface->GetData();
}

extern "C" int32_t AzDataSourceSurfaceGetStride(AzDataSourceSurfaceRef aSurface) {
    gfx::DataSourceSurface *gfxDataSourceSurface = static_cast<gfx::DataSourceSurface*>(aSurface);
    return gfxDataSourceSurface->Stride();
}

extern "C" AzScaledFontRef
AzCreateScaledFontForNativeFont(AzNativeFont *aNativeFont, AzFloat aSize) {
    gfx::NativeFont *gfxNativeFont = reinterpret_cast<gfx::NativeFont*>(aNativeFont);
    RefPtr<gfx::ScaledFont> font = gfx::Factory::CreateScaledFontForNativeFont(*gfxNativeFont, aSize);
    font->AddRef();
    return font;
}

extern "C" AzScaledFontRef
AzCreateScaledFontForTrueTypeData(uint8_t *aData, uint32_t aSize, uint32_t aFaceIndex, AzFloat aGlyphSize, AzFontType) {
    RefPtr<gfx::ScaledFont> font = new gfx::ScaledFontSkia(aData, aSize, aFaceIndex, aGlyphSize);
    font->AddRef();
    return font;
}

extern "C" void
AzReleaseScaledFont(AzScaledFontRef aFont) {
    gfx::ScaledFont *gfxFont = static_cast<gfx::ScaledFont*>(aFont);
    gfxFont->Release();
}

extern "C" void
AzDrawTargetSetTransform(AzDrawTargetRef aDrawTarget,
                         AzMatrix *aTransform) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Matrix *gfxMatrix = reinterpret_cast<gfx::Matrix*>(aTransform);
    gfxDrawTarget->SetTransform(*gfxMatrix);
}

extern "C" AzFontOptions*
AzCreateFontOptionsForName(char *aName, AzFontStyle aStyle) {
    #ifdef MOZ_ENABLE_FREETYPE
    gfx::FontOptions *options = new gfx::FontOptions;
    options->mName = std::string(aName);
    options->mStyle = static_cast<gfx::FontStyle>(aStyle);
    return options;
    #else
    abort();
    #endif
}

extern "C" void
AzDestroyFontOptions(AzFontOptions* aOptions) {
    #ifdef MOZ_ENABLE_FREETYPE
    gfx::FontOptions *fontOptions = reinterpret_cast<gfx::FontOptions*>(aOptions);
    delete fontOptions;
    #else
    abort();
    #endif
}

// FIXME: Needs to take a FillRule
extern "C" AzPathBuilderRef
AzCreatePathBuilder(AzDrawTargetRef aDrawTarget) {
  gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
  RefPtr<gfx::PathBuilder> gfxPathBuilder = gfxDrawTarget->CreatePathBuilder();
  gfxPathBuilder->AddRef();
  return gfxPathBuilder;
}

extern "C" void
AzReleasePathBuilder(AzPathBuilderRef aPathBuilder) {
  gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
  gfxPathBuilder->Release();
}

extern "C" void
AzPathBuilderMoveTo(AzPathBuilderRef aPathBuilder, const AzPoint *aPoint) {
  gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
  const gfx::Point *gfxPoint = reinterpret_cast<const gfx::Point*>(aPoint);
  gfxPathBuilder->MoveTo(*gfxPoint);
}

extern "C" void
AzPathBuilderLineTo(AzPathBuilderRef aPathBuilder, const AzPoint *aPoint) {
  gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
  const gfx::Point *gfxPoint = reinterpret_cast<const gfx::Point*>(aPoint);
  gfxPathBuilder->LineTo(*gfxPoint);
}

extern "C" void AzPathBuilderArc(AzPathBuilderRef aPathBuilder,
                                 const AzPoint *aOrigin,
                                 AzFloat aRadius,
                                 AzFloat aStartAngle,
                                 AzFloat aEndAngle,
                                 bool aAntiClockwise) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    const gfx::Point *gfxOrigin = reinterpret_cast<const gfx::Point*>(aOrigin);
    gfxPathBuilder->Arc(*gfxOrigin, aRadius, aStartAngle, aEndAngle, aAntiClockwise);
}

extern "C" void
AzPathBuilderBezierTo(AzPathBuilderRef aPathBuilder,
                      const AzPoint *aControlPoint1,
                      const AzPoint *aControlPoint2,
                      const AzPoint *aControlPoint3) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    const gfx::Point *gfxControlPoint1 = reinterpret_cast<const gfx::Point*>(aControlPoint1);
    const gfx::Point *gfxControlPoint2 = reinterpret_cast<const gfx::Point*>(aControlPoint2);
    const gfx::Point *gfxControlPoint3 = reinterpret_cast<const gfx::Point*>(aControlPoint3);
    gfxPathBuilder->BezierTo(*gfxControlPoint1, *gfxControlPoint2, *gfxControlPoint3);
}

extern "C" void
AzPathBuilderQuadraticBezierTo(AzPathBuilderRef aPathBuilder,
                               const AzPoint *aControlPoint,
                               const AzPoint *aEndPoint) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    const gfx::Point *gfxControlPoint = reinterpret_cast<const gfx::Point*>(aControlPoint);
    const gfx::Point *gfxEndPoint = reinterpret_cast<const gfx::Point*>(aEndPoint);
    gfxPathBuilder->QuadraticBezierTo(*gfxControlPoint, *gfxEndPoint);
}

extern "C" AzPoint
AzPathBuilderCurrentPoint(AzPathBuilderRef aPathBuilder) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    AzPoint p;
    p.x = gfxPathBuilder->CurrentPoint().x;
    p.y = gfxPathBuilder->CurrentPoint().y;
    return p;
}

extern "C" void
AzPathBuilderClose(AzPathBuilderRef aPathBuilder) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    gfxPathBuilder->Close();
}

extern "C" AzPathRef
AzPathBuilderFinish(AzPathBuilderRef aPathBuilder) {
    gfx::PathBuilder *gfxPathBuilder = static_cast<gfx::PathBuilder*>(aPathBuilder);
    RefPtr<gfx::Path> gfxPath = gfxPathBuilder->Finish();
    gfxPath->AddRef();
    return gfxPath;
}

extern "C" void
AzReleasePath(AzPathRef aPath) {
    gfx::Path *gfxPath = static_cast<gfx::Path*>(aPath);
    gfxPath->Release();
}

extern "C" AzLinearGradientPatternRef
AzCreateLinearGradientPattern(const AzPoint *aBegin,
                              const AzPoint *aEnd,
                              AzGradientStopsRef aStops,
                              const AzMatrix *aMatrix) {
    const gfx::Point *gfxBegin = reinterpret_cast<const gfx::Point*>(aBegin);
    const gfx::Point *gfxEnd = reinterpret_cast<const gfx::Point*>(aEnd);
    gfx::GradientStops *gfxStops = reinterpret_cast<gfx::GradientStops*>(aStops);
    const gfx::Matrix *gfxMatrix = reinterpret_cast<const gfx::Matrix*>(aMatrix);
    gfx::LinearGradientPattern* gfxLinearGradientPattern = new
        gfx::LinearGradientPattern(*gfxBegin, *gfxEnd, gfxStops, *gfxMatrix);
    return gfxLinearGradientPattern;
}

extern "C" AzLinearGradientPatternRef
AzCloneLinearGradientPattern(AzLinearGradientPatternRef aPattern) {
    gfx::LinearGradientPattern *gfxLinearGradientPattern = static_cast<gfx::LinearGradientPattern*>(aPattern);
    return new gfx::LinearGradientPattern(gfxLinearGradientPattern->mBegin,
                                          gfxLinearGradientPattern->mEnd,
                                          gfxLinearGradientPattern->mStops,
                                          gfxLinearGradientPattern->mMatrix);
}

extern "C" AzRadialGradientPatternRef
AzCreateRadialGradientPattern(const AzPoint *aCenter1,
                              const AzPoint *aCenter2,
                              AzFloat aRadius1,
                              AzFloat aRadius2,
                              AzGradientStopsRef aStops,
                              const AzMatrix *aMatrix) {
    const gfx::Point *gfxCenter1 = reinterpret_cast<const gfx::Point*>(aCenter1);
    const gfx::Point *gfxCenter2 = reinterpret_cast<const gfx::Point*>(aCenter2);
    gfx::GradientStops *gfxStops = reinterpret_cast<gfx::GradientStops*>(aStops);
    const gfx::Matrix *gfxMatrix = reinterpret_cast<const gfx::Matrix*>(aMatrix);
    gfx::RadialGradientPattern* gfxRadialGradientPattern = new
        gfx::RadialGradientPattern(*gfxCenter1, *gfxCenter2, aRadius1, aRadius2, gfxStops, *gfxMatrix);
    return gfxRadialGradientPattern;
}

extern "C" AzRadialGradientPatternRef
AzCloneRadialGradientPattern(AzRadialGradientPatternRef aPattern) {
    gfx::RadialGradientPattern *gfxRadialGradientPattern = static_cast<gfx::RadialGradientPattern*>(aPattern);
    return new gfx::RadialGradientPattern(gfxRadialGradientPattern->mCenter1,
                                          gfxRadialGradientPattern->mCenter2,
                                          gfxRadialGradientPattern->mRadius1,
                                          gfxRadialGradientPattern->mRadius2,
                                          gfxRadialGradientPattern->mStops,
                                          gfxRadialGradientPattern->mMatrix);
}

extern "C" AzSurfacePatternRef
AzCreateSurfacePattern(AzSourceSurfaceRef aSurface) {
    gfx::SourceSurface *gfxSourceSurface = reinterpret_cast<gfx::SourceSurface*>(aSurface);
    gfx::SurfacePattern* gfxSurfacePattern = new
        gfx::SurfacePattern(gfxSourceSurface, gfx::ExtendMode::CLAMP);
    return gfxSurfacePattern;
}

extern "C" AzSurfacePatternRef
AzCloneSurfacePattern(AzSurfacePatternRef aPattern) {
    gfx::SurfacePattern *gfxSurfacePattern = static_cast<gfx::SurfacePattern*>(aPattern);
    return new gfx::SurfacePattern(gfxSurfacePattern->mSurface,
                                   gfxSurfacePattern->mExtendMode,
                                   gfxSurfacePattern->mMatrix,
                                   gfxSurfacePattern->mFilter,
                                   gfxSurfacePattern->mSamplingRect);
}

extern "C" void
AzReleasePattern(AzPatternRef aPattern) {
    gfx::Pattern *gfxPattern = reinterpret_cast<gfx::Pattern*>(aPattern);
    delete gfxPattern;
}

extern "C" void
AzReleaseGradientStops(AzGradientStopsRef aStops) {
    gfx::GradientStops *gfxStops = reinterpret_cast<gfx::GradientStops*>(aStops);
    gfxStops->Release();
}

extern "C" AzFilterNodeRef
AzDrawTargetCreateFilter(AzDrawTargetRef aDrawTarget, AzFilterType aType) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::FilterType gfxFilterType = static_cast<gfx::FilterType>(aType);
    return gfxDrawTarget->CreateFilter(gfxFilterType).drop();
}

extern "C" void
AzReleaseFilterNode(AzFilterNodeRef aFilter) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfxFilterNode->Release();
}

extern "C" void
AzFilterNodeSetSourceSurfaceInput(AzFilterNodeRef aFilter,
                                  uint32_t aIndex,
                                  AzSourceSurfaceRef aSurface) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfx::SourceSurface *gfxSourceSurface = reinterpret_cast<gfx::SourceSurface*>(aSurface);
    gfxFilterNode->SetInput(aIndex, gfxSourceSurface);
}

extern "C" void
AzFilterNodeSetFilterNodeInput(AzFilterNodeRef aFilter,
                               uint32_t aIndex,
                               AzFilterNodeRef aInputFilter) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfx::FilterNode *gfxInputFilterNode = reinterpret_cast<gfx::FilterNode*>(aInputFilter);
    gfxFilterNode->SetInput(aIndex, gfxInputFilterNode);
}

extern "C" void
AzFilterNodeSetFloatAttribute(AzFilterNodeRef aFilter, uint32_t aIndex, AzFloat aValue) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfxFilterNode->SetAttribute(aIndex, aValue);
}

extern "C" void
AzFilterNodeSetFloatArrayAttribute(AzFilterNodeRef aFilter,
                                   uint32_t aIndex,
                                   const AzFloat *aFloats,
                                   uint32_t aSize) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfxFilterNode->SetAttribute(aIndex, aFloats, aSize);
}

extern "C" void
AzFilterNodeSetColorAttribute(AzFilterNodeRef aFilter, uint32_t aIndex, const AzColor *aValue) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    const gfx::Color *gfxColor = reinterpret_cast<const gfx::Color*>(aValue);
    gfxFilterNode->SetAttribute(aIndex, aValue);
}

extern "C" void
AzFilterNodeSetMatrix5x4Attribute(AzFilterNodeRef aFilter,
                                  uint32_t aIndex,
                                  const AzMatrix5x4 *aValue) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    const gfx::Matrix5x4 *gfxMatrix5x4 = reinterpret_cast<const gfx::Matrix5x4*>(aValue);
    gfxFilterNode->SetAttribute(aIndex, *gfxMatrix5x4);
}

extern "C" void
AzFilterNodeSetBoolAttribute(AzFilterNodeRef aFilter, uint32_t aIndex, bool aValue) {
    gfx::FilterNode *gfxFilterNode = reinterpret_cast<gfx::FilterNode*>(aFilter);
    gfxFilterNode->SetAttribute(aIndex, aValue);
}



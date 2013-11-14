/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "azure-c.h"
#include "mozilla/gfx/2D.h"

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
#define CHECK_ENUM(name) assert((int)AZ_##name == (int)gfx::name)

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

    CHECK_ENUM(SURFACE_DATA);
    CHECK_ENUM(SURFACE_D2D1_BITMAP);
    CHECK_ENUM(SURFACE_D2D1_DRAWTARGET);
    CHECK_ENUM(SURFACE_CAIRO);
    CHECK_ENUM(SURFACE_CAIRO_IMAGE);
    CHECK_ENUM(SURFACE_COREGRAPHICS_IMAGE);
    CHECK_ENUM(SURFACE_COREGRAPHICS_CGCONTEXT);
    CHECK_ENUM(SURFACE_SKIA);
    CHECK_ENUM(SURFACE_DUAL_DT);

    CHECK_ENUM(FORMAT_B8G8R8A8);
    CHECK_ENUM(FORMAT_B8G8R8X8);
    CHECK_ENUM(FORMAT_R5G6B5);
    CHECK_ENUM(FORMAT_A8);

    CHECK_ENUM(BACKEND_NONE);
    CHECK_ENUM(BACKEND_DIRECT2D);
    CHECK_ENUM(BACKEND_COREGRAPHICS);
    CHECK_ENUM(BACKEND_CAIRO);
    CHECK_ENUM(BACKEND_SKIA);

    CHECK_ENUM(FONT_DWRITE);
    CHECK_ENUM(FONT_GDI);
    CHECK_ENUM(FONT_MAC);
    CHECK_ENUM(FONT_SKIA);
    CHECK_ENUM(FONT_CAIRO);
    CHECK_ENUM(FONT_COREGRAPHICS);

    CHECK_ENUM(NATIVE_SURFACE_D3D10_TEXTURE);
    CHECK_ENUM(NATIVE_SURFACE_CAIRO_SURFACE);
    CHECK_ENUM(NATIVE_SURFACE_CGCONTEXT);

    CHECK_ENUM(NATIVE_FONT_DWRITE_FONT_FACE);
    CHECK_ENUM(NATIVE_FONT_GDI_FONT_FACE);
    CHECK_ENUM(NATIVE_FONT_MAC_FONT_FACE);
    CHECK_ENUM(NATIVE_FONT_SKIA_FONT_FACE);
    CHECK_ENUM(NATIVE_FONT_CAIRO_FONT_FACE);

    CHECK_ENUM(OP_OVER);
    CHECK_ENUM(OP_ADD);
    CHECK_ENUM(OP_ATOP);
    CHECK_ENUM(OP_OUT);
    CHECK_ENUM(OP_IN);
    CHECK_ENUM(OP_SOURCE);
    CHECK_ENUM(OP_DEST_IN);
    CHECK_ENUM(OP_DEST_OUT);
    CHECK_ENUM(OP_DEST_OVER);
    CHECK_ENUM(OP_DEST_ATOP);
    CHECK_ENUM(OP_XOR);
    CHECK_ENUM(OP_MULTIPLY);
    CHECK_ENUM(OP_SCREEN);
    CHECK_ENUM(OP_OVERLAY);
    /* ... */
    CHECK_ENUM(OP_LUMINOSITY);
    CHECK_ENUM(OP_COUNT);

    CHECK_ENUM(EXTEND_CLAMP);
    CHECK_ENUM(EXTEND_REPEAT);
    CHECK_ENUM(EXTEND_REFLECT);

    CHECK_ENUM(FILL_WINDING);
    CHECK_ENUM(FILL_EVEN_ODD);

    CHECK_ENUM(AA_NONE);
    CHECK_ENUM(AA_GRAY);
    CHECK_ENUM(AA_SUBPIXEL);

    CHECK_ENUM(SNAP_NONE);
    CHECK_ENUM(SNAP_ALIGNED);

    CHECK_ENUM(FILTER_LINEAR);
    CHECK_ENUM(FILTER_POINT);

    CHECK_ENUM(PATTERN_COLOR);
    CHECK_ENUM(PATTERN_SURFACE);
    CHECK_ENUM(PATTERN_LINEAR_GRADIENT);
    CHECK_ENUM(PATTERN_RADIAL_GRADIENT);

    CHECK_ENUM(JOIN_BEVEL);
    CHECK_ENUM(JOIN_ROUND);
    CHECK_ENUM(JOIN_MITER);
    CHECK_ENUM(JOIN_MITER_OR_BEVEL);

    CHECK_ENUM(CAP_BUTT);
    CHECK_ENUM(CAP_ROUND);
    CHECK_ENUM(CAP_SQUARE);

    CHECK_ENUM(SAMPLING_UNBOUNDED);
    CHECK_ENUM(SAMPLING_BOUNDED);

    assert((int)AZ_eSideTop == (int)css::eSideTop);
    assert((int)AZ_eSideRight == (int)css::eSideRight);
    assert((int)AZ_eSideBottom == (int)css::eSideBottom);
    assert((int)AZ_eSideLeft == (int)css::eSideLeft);
}


extern "C" AzColorPatternRef
AzCreateColorPattern(AzColor *aColor) {
    gfx::Color *gfxColor = reinterpret_cast<gfx::Color*>(aColor);
    gfx::ColorPattern *gfxColorPattern = new gfx::ColorPattern(*gfxColor);
    return gfxColorPattern;
}

extern "C" void
AzReleaseColorPattern(AzColorPatternRef aColorPattern) {
    gfx::ColorPattern *gfxColorPattern = static_cast<gfx::ColorPattern*>(aColorPattern);
    delete gfxColorPattern;
}

extern "C" AzSkiaSharedGLContextRef
AzCreateSkiaSharedGLContext(AzGLNativeContextRef aNativeContext, AzIntSize *aSize) {
    GrGLNativeContext* nativeContext = reinterpret_cast<GrGLNativeContext*>(aNativeContext);
    SkNativeSharedGLContext *sharedGLContext = new SkNativeSharedGLContext(*nativeContext);
    if (sharedGLContext == NULL) {
        return NULL;
    }
    if (!sharedGLContext->init(aSize->width, aSize->height)) {
        return NULL;
    }
    return sharedGLContext;
}

extern "C" void
AzRetainSkiaSharedGLContext(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    sharedGLContext->AddRef();
}

extern "C" void
AzReleaseSkiaSharedGLContext(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    sharedGLContext->Release();
}

extern "C" unsigned int
AzSkiaSharedGLContextGetFBOID(AzSkiaSharedGLContextRef aGLContext) {
   SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
   return sharedGLContext->getFBOID();
}

extern "C" AzSkiaGrGLSharedSurfaceRef
AzSkiaSharedGLContextStealSurface(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    return reinterpret_cast<void*>(sharedGLContext->stealSurface());
}

extern "C" AzSkiaGrContextRef
AzSkiaSharedGLContextGetGrContext(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    return sharedGLContext->getGrContext();
}

extern "C" void
AzSkiaSharedGLContextMakeCurrent(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    sharedGLContext->makeCurrent();
}

extern "C" void
AzSkiaSharedGLContextFlush(AzSkiaSharedGLContextRef aGLContext) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    sharedGLContext->flush();
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
AzCreateSkiaDrawTargetForFBO(AzSkiaSharedGLContextRef aGLContext, AzIntSize *aSize, AzSurfaceFormat aFormat) {
    SkNativeSharedGLContext *sharedGLContext = static_cast<SkNativeSharedGLContext*>(aGLContext);
    GrContext *grContext = sharedGLContext->getGrContext();
    gfx::IntSize *size = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat surfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::DrawTarget> target = gfx::Factory::CreateSkiaDrawTargetForFBO(sharedGLContext->getFBOID(),
                                                                              grContext,
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
AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget, AzRect *aRect,
		     AzPatternRef aPattern) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::Rect *gfxRect = reinterpret_cast<gfx::Rect*>(aRect);
    gfx::Pattern *gfxPattern = static_cast<gfx::Pattern*>(aPattern);
    gfxDrawTarget->FillRect(*gfxRect, *gfxPattern);
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

extern "C" AzSourceSurfaceRef
AzDrawTargetGetSnapshot(AzDrawTargetRef aDrawTarget) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    RefPtr<gfx::SourceSurface> gfxSourceSurface = gfxDrawTarget->Snapshot();
    gfxSourceSurface->AddRef();
    return gfxSourceSurface;
}

extern "C" AzSourceSurfaceRef
AzDrawTargetCreateSourceSurfaceFromData(AzDrawTargetRef aDrawTarget,
                                        unsigned char *aData,
                                        AzIntSize *aSize,
                                        int32_t aStride,
                                        AzSurfaceFormat aFormat) {
    gfx::DrawTarget *gfxDrawTarget = static_cast<gfx::DrawTarget*>(aDrawTarget);
    gfx::IntSize *gfxSize = reinterpret_cast<gfx::IntSize*>(aSize);
    gfx::SurfaceFormat gfxSurfaceFormat = static_cast<gfx::SurfaceFormat>(aFormat);
    RefPtr<gfx::SourceSurface> gfxSourceSurface = gfxDrawTarget->CreateSourceSurfaceFromData(aData, *gfxSize, aStride, gfxSurfaceFormat);
    gfxSourceSurface->AddRef();
    return gfxSourceSurface;
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
    return static_cast<AzSurfaceFormat>(gfxSourceSurface->GetFormat());
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
AzCreateFontOptions(char *aName, AzFontStyle aStyle) {
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

extern "C" AzGLContext
AzSkiaGetCurrentGLContext() {
    return SkNativeSharedGLContext::GetCurrent();
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

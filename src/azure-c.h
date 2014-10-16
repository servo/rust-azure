/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_AZURE_C_H_
#define MOZILLA_GFX_AZURE_C_H_

#include "GrContext.h"

#include "SkNativeSharedGLContext.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// A function that we use to test that our definitions match the C++
void AzSanityCheck();


// FIXME: This stuff is copy pasted from the azure headers

typedef void* AzGradientStopsRef;
typedef void* AzSkiaSharedGLContextRef;
typedef void* AzSkiaSharedGrGLContextRef;
typedef void* AzSkiaGrContextRef;
typedef void* AzSkiaGrGLSharedSurfaceRef;
typedef void* AzSkiaGrGLSharedContextRef;
typedef void* AzDrawTargetRef;
typedef void* AzPatternRef;
typedef void* AzColorPatternRef;
typedef void* AzScaledFontRef;
typedef void* AzGlyphRenderingOptionsRef;
typedef void* AzSourceSurfaceRef;
typedef void* AzDrawSurfaceOptionsRef;
typedef void* AzDataSourceSurfaceRef;
typedef void* AzGLContextMetadataRef;
typedef void* AzPathBuilderRef;
typedef void* AzPathRef;
typedef void* AzLinearGradientPatternRef;

/* Types.h */

typedef float AzFloat;

enum AzSurfaceType
{
  AZ_SURFACE_DATA, /* Data surface - bitmap in memory */
  AZ_SURFACE_D2D1_BITMAP, /* Surface wrapping a ID2D1Bitmap */
  AZ_SURFACE_D2D1_DRAWTARGET, /* Surface made from a D2D draw target */
  AZ_SURFACE_CAIRO, /* Surface wrapping a cairo surface */
  AZ_SURFACE_CAIRO_IMAGE, /* Data surface wrapping a cairo image surface */
  AZ_SURFACE_COREGRAPHICS_IMAGE, /* Surface wrapping a CoreGraphics Image */
  AZ_SURFACE_COREGRAPHICS_CGCONTEXT, /* Surface wrapping a CG context */
  AZ_SURFACE_SKIA, /* Surface wrapping a Skia bitmap */
  AZ_SURFACE_DUAL_DT /* Snapshot of a dual drawtarget */
};

enum AzSurfaceFormat
{
  AZ_FORMAT_B8G8R8A8,
  AZ_FORMAT_B8G8R8X8,
  AZ_FORMAT_R5G6B5,
  AZ_FORMAT_A8
};

enum AzBackendType
{
  AZ_BACKEND_NONE,
  AZ_BACKEND_DIRECT2D,
  AZ_BACKEND_COREGRAPHICS,
  AZ_BACKEND_COREGRAPHICS_ACCELERATED,
  AZ_BACKEND_CAIRO,
  AZ_BACKEND_SKIA
};

enum AzFontType
{
  AZ_FONT_DWRITE,
  AZ_FONT_GDI,
  AZ_FONT_MAC,
  AZ_FONT_SKIA,
  AZ_FONT_CAIRO,
  AZ_FONT_COREGRAPHICS
};

enum AzNativeSurfaceType
{
  AZ_NATIVE_SURFACE_D3D10_TEXTURE,
  AZ_NATIVE_SURFACE_CAIRO_SURFACE,
  AZ_NATIVE_SURFACE_CGCONTEXT
};

enum AzNativeFontType
{
  AZ_NATIVE_FONT_DWRITE_FONT_FACE,
  AZ_NATIVE_FONT_GDI_FONT_FACE,
  AZ_NATIVE_FONT_MAC_FONT_FACE,
  AZ_NATIVE_FONT_SKIA_FONT_FACE,
  AZ_NATIVE_FONT_CAIRO_FONT_FACE
};

enum AzFontStyle
{
  AZ_FONT_STYLE_NORMAL,
  AZ_FONT_STYLE_ITALIC,
  AZ_FONT_STYLE_BOLD,
  AZ_FONT_STYLE_BOLD_ITALIC
};

enum AzCompositionOp {
  AZ_OP_OVER,
  AZ_OP_ADD,
  AZ_OP_ATOP,
  AZ_OP_OUT,
  AZ_OP_IN,
  AZ_OP_SOURCE,
  AZ_OP_DEST_IN,
  AZ_OP_DEST_OUT,
  AZ_OP_DEST_OVER,
  AZ_OP_DEST_ATOP,
  AZ_OP_XOR,
  AZ_OP_MULTIPLY,
  AZ_OP_SCREEN,
  AZ_OP_OVERLAY,
  AZ_OP_DARKEN,
  AZ_OP_LIGHTEN,
  AZ_OP_COLOR_DODGE,
  AZ_OP_COLOR_BURN,
  AZ_OP_HARD_LIGHT,
  AZ_OP_SOFT_LIGHT,
  AZ_OP_DIFFERENCE,
  AZ_OP_EXCLUSION,
  AZ_OP_HUE,
  AZ_OP_SATURATION,
  AZ_OP_COLOR,
  AZ_OP_LUMINOSITY,
  AZ_OP_COUNT
};

enum AzExtendMode {
  AZ_EXTEND_CLAMP,
  AZ_EXTEND_REPEAT,
  AZ_EXTEND_REFLECT
};

enum AzFillRule {
  AZ_FILL_WINDING,
  AZ_FILL_EVEN_ODD
};

enum AzAntialiasMode {
  AZ_AA_NONE,
  AZ_AA_GRAY,
  AZ_AA_SUBPIXEL
};

enum AzSnapping {
  AZ_SNAP_NONE,
  AZ_SNAP_ALIGNED
};

enum AzFilter {
  AZ_FILTER_LINEAR,
  AZ_FILTER_POINT
};

enum AzPatternType {
  AZ_PATTERN_COLOR,
  AZ_PATTERN_SURFACE,
  AZ_PATTERN_LINEAR_GRADIENT,
  AZ_PATTERN_RADIAL_GRADIENT
};

enum AzJoinStyle {
  AZ_JOIN_BEVEL,
  AZ_JOIN_ROUND,
  AZ_JOIN_MITER,
  AZ_JOIN_MITER_OR_BEVEL
};

enum AzCapStyle {
  AZ_CAP_BUTT,
  AZ_CAP_ROUND,
  AZ_CAP_SQUARE
};

enum AzSamplingBounds {
  AZ_SAMPLING_UNBOUNDED,
  AZ_SAMPLING_BOUNDED
};

enum AzSide {
  AZ_eSideTop,
  AZ_eSideRight,
  AZ_eSideBottom,
  AZ_eSideLeft
};

typedef struct _AzColor {
    AzFloat r, g, b, a;
} AzColor;

typedef struct _AzGradientStop {
    AzFloat offset;
    AzColor color;
} AzGradientStop;


/* Rect.h */

typedef struct _AzIntRect {
    int32_t x, y, width, height;
} AzIntRect;

typedef struct _AzRect {
    AzFloat x, y, width, height;
} AzRect;


/* Point.h */

typedef struct _AzIntPoint {
  int32_t x, y;
} AzIntPoint;

typedef struct _AzPoint {
  AzFloat x, y;
} AzPoint;

typedef struct _AzIntSize {
  int32_t width, height;
} AzIntSize;

typedef struct _AzSize {
  AzFloat width, height;
} AzSize;


/* Matrix.h */

typedef struct _AzMatrix {
  AzFloat _11, _12;
  AzFloat _21, _22;
  AzFloat _31, _32;
} AzMatrix;


/* 2D.h */

// FIXME: rust-bindgen can't handle bitfields
typedef struct _AzDrawOptions {
  AzFloat mAlpha;
  uint16_t fields;
  /*
  enum AzCompositionOp mCompositionOp : 8;
  enum AzAntialiasMode mAntialiasMode : 2;
  enum AzSnapping mSnapping : 1;
  */
} AzDrawOptions;

// FIXME: Again, bitfields
typedef struct _AzStrokeOptions {
  AzFloat mLineWidth;
  AzFloat mMiterLimit;
  const AzFloat* mDashPattern;
  size_t mDashLength;
  AzFloat mDashOffset;
  uint8_t fields;
  /*
    enum AzJoinStyle mLineJoin : 4;
    enum AzCapStyle mLineCap : 3;
   */
} AzStrokeOptions;

// FIXME: Guess what? Bitfields
typedef struct _AzDrawSurfaceOptions {
  uint32_t fields;
  /*
  enum Filter mFilter : 3;
  enum Filter mSamplingBounds : 1;
  */
} AzDrawSurfaceOptions;

typedef struct _AzGlyph {
  uint32_t mIndex;
  AzPoint mPosition;
} AzGlyph;

typedef struct _AzGlyphBuffer {
  const AzGlyph *mGlyphs;
  uint32_t mNumGlyphs;
} AzGlyphBuffer;

typedef struct _AzNativeFont {
  enum AzNativeFontType mType;
  void *mFont;
} AzNativeFont;

typedef GrGLSharedContext AzGLContext;
typedef GrGLNativeContext* AzGLNativeContextRef;

AzColorPatternRef AzCreateColorPattern(AzColor *aColor);

AzSkiaSharedGLContextRef AzCreateSkiaSharedGLContext(AzGLNativeContextRef aNativeContext,
                                                     AzIntSize *aSize);
void AzRetainSkiaSharedGLContext(AzSkiaSharedGLContextRef aGLContext);
void AzReleaseSkiaSharedGLContext(AzSkiaSharedGLContextRef aGLContext);
unsigned int AzSkiaSharedGLContextGetFBOID(AzSkiaSharedGLContextRef aGLContext);
AzSkiaGrContextRef AzSkiaSharedGLContextGetGrContext(AzSkiaSharedGLContextRef aGLContext);
void AzSkiaSharedGLContextMakeCurrent(AzSkiaSharedGLContextRef aGLContext);
void AzSkiaSharedGLContextFlush(AzSkiaSharedGLContextRef aGLContext);

AzDrawTargetRef AzCreateDrawTarget(AzBackendType aBackend,
                                   AzIntSize *aSize,
                                   AzSurfaceFormat aFormat);
AzDrawTargetRef AzCreateDrawTargetForData(AzBackendType aBackend,
                                          unsigned char *aData,
                                          AzIntSize *aSize,
                                          int32_t aStride,
                                          AzSurfaceFormat aFormat);
AzDrawTargetRef AzCreateSkiaDrawTragetForFBO(AzSkiaSharedGLContextRef aGLContext,
                                             AzIntSize *aSize,
                                             AzSurfaceFormat aFormat);

void AzRetainDrawTarget(AzDrawTargetRef aTarget);

void AzReleaseDrawTarget(AzDrawTargetRef aTarget);

AzIntSize AzDrawTargetGetSize(AzDrawTargetRef aDrawTarget);
void AzDrawTargetFlush(AzDrawTargetRef aDrawTarget);
void AzDrawTargetClearRect(AzDrawTargetRef aDrawTarget, AzRect *aRect);
void AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget,
			              AzRect* aRect,
			              AzPatternRef aPattern,
                          AzDrawOptions* aDrawOptions);
void AzDrawTargetStrokeRect(AzDrawTargetRef aDrawTarget,
			    AzRect *aRect,
			    AzPatternRef aPattern,
			    AzStrokeOptions *aStrokeOptions,
			    AzDrawOptions *aDrawOptions);
void AzDrawTargetStrokeLine(AzDrawTargetRef aDrawTarget,
			                AzPoint *aStart,
			                AzPoint *aEnd,
			                AzPatternRef aPattern,
			                AzStrokeOptions *aStrokeOptions,
			                AzDrawOptions *aDrawOptions);
void AzDrawTargetFillGlyphs(AzDrawTargetRef aDrawTarget,
                            AzScaledFontRef aFont,
                            AzGlyphBuffer *aGlyphBuffer,
                            AzPatternRef aPattern,
                            AzDrawOptions *aOptions,
                            AzGlyphRenderingOptionsRef aRenderingOptions);
void AzDrawTargetDrawSurface(AzDrawTargetRef aDrawTarget,
                             AzSourceSurfaceRef aSurface,
                             AzRect *aDest,
                             AzRect *aSource,
                             AzDrawSurfaceOptionsRef aSurfOptions,
                             AzDrawOptions *aOptions);
AzSourceSurfaceRef AzDrawTargetGetSnapshot(AzDrawTargetRef aDrawTarget);
AzSourceSurfaceRef AzDrawTargetCreateSourceSurfaceFromData(AzDrawTargetRef aDrawTarget,
                                                           const unsigned char *aData,
                                                           AzIntSize *aSize,
                                                           int32_t aStride,
                                                           AzSurfaceFormat aFormat);
AzGradientStopsRef AzDrawTargetCreateGradientStops(AzDrawTargetRef aDrawTarget,
                                                   AzGradientStop *aStops,
                                                   uint32_t aNumStops,
                                                   AzExtendMode aExtendMode);

void AzReleaseSourceSurface(AzSourceSurfaceRef aSurface);
AzIntSize AzSourceSurfaceGetSize(AzSourceSurfaceRef aSurface);
AzSurfaceFormat AzSourceSurfaceGetFormat(AzSourceSurfaceRef aSurface);
AzDataSourceSurfaceRef AzSourceSurfaceGetDataSurface(AzSourceSurfaceRef aSurface);
uint8_t *AzDataSourceSurfaceGetData(AzDataSourceSurfaceRef aSurface);
int32_t AzDataSourceSurfaceGetStride(AzDataSourceSurfaceRef aSurface);

AzPathBuilderRef AzCreatePathBuilder(AzDrawTargetRef aDrawTarget);
void AzReleasePathBuilder(AzPathBuilderRef aPathBuilder);
void AzPathBuilderMoveTo(AzPathBuilderRef aPathBuilder, const AzPoint *aPoint);
void AzPathBuilderLineTo(AzPathBuilderRef aPathBuilder, const AzPoint *aPoint);
void AzPathBuilderArc(AzPathBuilderRef aPathBuilder,
                      const AzPoint *aOrigin,
                      AzFloat aRadius,
                      AzFloat aStartAngle,
                      AzFloat aEndAngle,
                      bool aAntiClockwise);
AzPathRef AzPathBuilderFinish(AzPathBuilderRef aPathBuilder);

AzLinearGradientPatternRef AzCreateLinearGradientPattern(const AzPoint *aBegin,
                                                         const AzPoint *aEnd,
                                                         AzGradientStopsRef aStops,
                                                         const AzMatrix *aMatrix);
void AzReleasePattern(AzPatternRef aPattern);

void AzReleaseGradientStops(AzGradientStopsRef aStops);

/* Factory.h */

void AzReleaseScaledFont(AzScaledFontRef aFont);

/* Helpers */
typedef void AzFontOptions;
AzFontOptions* AzCreateFontOptionsForName(char *aName, AzFontStyle aStyle);
AzFontOptions* AzCreateFontOptionsForData(uint8_t *aFontData, uint32_t aFontDataSize);
void AzDestroyFontOptions(AzFontOptions* aOptions);

AzGLContext AzSkiaGetCurrentGLContext();

#ifdef __cplusplus
}
#endif

#endif /* MOZILLA_GFX_AZURE_C_H */

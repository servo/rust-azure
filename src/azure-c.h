/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_AZURE_C_H_
#define MOZILLA_GFX_AZURE_C_H_

#include "skia-c.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// A function that we use to test that our definitions match the C++
void AzSanityCheck();


// FIXME: This stuff is copy pasted from the azure headers

typedef void* AzGradientStopsRef;
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
typedef void* AzRadialGradientPatternRef;
typedef void* AzSurfacePatternRef;
typedef void* AzFilterNodeRef;

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
  AZ_SURFACE_DUAL_DT, /* Snapshot of a dual drawtarget */
  AZ_SURFACE_D2D1_1_IMAGE, /* A D2D 1.1 ID2D1Image SourceSurface */
  AZ_SURFACE_RECORDING, /* Surface used for recording */
  AZ_SURFACE_NVPR_TEXTURE, /* Surface wrapping an OpenGL texture, used by NV_path_rendering */
  AZ_SURFACE_TILED /* Surface from a tiled DrawTarget */
};

enum AzSurfaceFormat
{
  AZ_FORMAT_B8G8R8A8,
  AZ_FORMAT_B8G8R8X8,
  AZ_FORMAT_R8G8B8A8,
  AZ_FORMAT_R8G8B8X8,
  AZ_FORMAT_R5G6B5,
  AZ_FORMAT_A8,
  AZ_FORMAT_YUV,
  AZ_FORMAT_UNKNOWN
};

enum AzFilterType {
  AZ_FILTER_TYPE_BLEND,
  AZ_FILTER_TYPE_TRANSFORM,
  AZ_FILTER_TYPE_MORPHOLOGY,
  AZ_FILTER_TYPE_COLOR_MATRIX,
  AZ_FILTER_TYPE_FLOOD,
  AZ_FILTER_TYPE_TILE,
  AZ_FILTER_TYPE_TABLE_TRANSFER,
  AZ_FILTER_TYPE_DISCRETE_TRANSFER,
  AZ_FILTER_TYPE_LINEAR_TRANSFER,
  AZ_FILTER_TYPE_GAMMA_TRANSFER,
  AZ_FILTER_TYPE_CONVOLVE_MATRIX,
  AZ_FILTER_TYPE_DISPLACEMENT_MAP,
  AZ_FILTER_TYPE_TURBULENCE,
  AZ_FILTER_TYPE_ARITHMETIC_COMBINE,
  AZ_FILTER_TYPE_COMPOSITE,
  AZ_FILTER_TYPE_DIRECTIONAL_BLUR,
  AZ_FILTER_TYPE_GAUSSIAN_BLUR,
  AZ_FILTER_TYPE_POINT_DIFFUSE,
  AZ_FILTER_TYPE_POINT_SPECULAR,
  AZ_FILTER_TYPE_SPOT_DIFFUSE,
  AZ_FILTER_TYPE_SPOT_SPECULAR,
  AZ_FILTER_TYPE_DISTANT_DIFFUSE,
  AZ_FILTER_TYPE_DISTANT_SPECULAR,
  AZ_FILTER_TYPE_CROP,
  AZ_FILTER_TYPE_PREMULTIPLY,
  AZ_FILTER_TYPE_UNPREMULTIPLY
};

enum AzBackendType
{
  AZ_BACKEND_NONE,
  AZ_BACKEND_DIRECT2D,
  AZ_BACKEND_COREGRAPHICS,
  AZ_BACKEND_COREGRAPHICS_ACCELERATED,
  AZ_BACKEND_CAIRO,
  AZ_BACKEND_SKIA,
  AZ_BACKEND_RECORDING,
  AZ_BACKEND_DIRECT2D1_1,
  AZ_BACKEND_NVPR
};

enum AzFontType
{
  AZ_FONT_DWRITE,
  AZ_FONT_GDI,
  AZ_FONT_MAC,
  AZ_FONT_SKIA,
  AZ_FONT_CAIRO,
  AZ_FONT_COREGRAPHICS,
  AZ_FONT_NVPR
};

enum AzNativeSurfaceType
{
  AZ_NATIVE_SURFACE_D3D10_TEXTURE,
  AZ_NATIVE_SURFACE_CAIRO_SURFACE,
  AZ_NATIVE_SURFACE_CAIRO_CONTEXT,
  AZ_NATIVE_SURFACE_CGCONTEXT,
  AZ_NATIVE_SURFACE_CGCONTEXT_ACCELERATED,
  AZ_NATIVE_SURFACE_OPENGL_TEXTURE
};

enum AzNativeFontType
{
  AZ_NATIVE_FONT_DWRITE_FONT_FACE,
  AZ_NATIVE_FONT_GDI_FONT_FACE,
  AZ_NATIVE_FONT_MAC_FONT_FACE,
  AZ_NATIVE_FONT_SKIA_FONT_FACE,
  AZ_NATIVE_FONT_CAIRO_FONT_FACE,
  AZ_NATIVE_FONT_NVPR_FONT_FACE
};

enum AzFontStyle
{
  AZ_FONT_STYLE_NORMAL,
  AZ_FONT_STYLE_ITALIC,
  AZ_FONT_STYLE_BOLD,
  AZ_FONT_STYLE_BOLD_ITALIC
};

enum AzCompositionOp: uint8_t {
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

enum AzAntialiasMode: uint8_t {
  AZ_AA_NONE,
  AZ_AA_GRAY,
  AZ_AA_SUBPIXEL,
  AZ_AA_DEFAULT
};

enum AzFilter {
  AZ_FILTER_GOOD,
  AZ_FILTER_LINEAR,
  AZ_FILTER_POINT
};

enum AzPatternType {
  AZ_PATTERN_COLOR,
  AZ_PATTERN_SURFACE,
  AZ_PATTERN_LINEAR_GRADIENT,
  AZ_PATTERN_RADIAL_GRADIENT
};

enum AzJoinStyle: uint8_t {
  AZ_JOIN_BEVEL,
  AZ_JOIN_ROUND,
  AZ_JOIN_MITER,
  AZ_JOIN_MITER_OR_BEVEL
};

enum AzCapStyle: uint8_t {
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

typedef struct _AzMatrix5x4 {
  AzFloat _11, _12, _13, _14;
  AzFloat _21, _22, _23, _24;
  AzFloat _31, _32, _33, _34;
  AzFloat _41, _42, _43, _44;
  AzFloat _51, _52, _53, _54;
} AzMatrix5x4;

/* 2D.h */

typedef struct _AzDrawOptions {
  AzFloat mAlpha;
  AzCompositionOp mCompositionOp;
  AzAntialiasMode mAntialiasMode;
} AzDrawOptions;

typedef struct _AzStrokeOptions {
  AzFloat mLineWidth;
  AzFloat mMiterLimit;
  const AzFloat* mDashPattern;
  size_t mDashLength;
  AzFloat mDashOffset;
  AzJoinStyle mLineJoin;
  AzCapStyle mLineCap;
} AzStrokeOptions;

typedef struct _AzDrawSurfaceOptions {
  int8_t filter;
  int8_t samplingBounds;
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

AzColorPatternRef AzCreateColorPattern(AzColor *aColor);

AzDrawTargetRef AzCreateDrawTarget(AzBackendType aBackend,
                                   AzIntSize *aSize,
                                   AzSurfaceFormat aFormat);
AzDrawTargetRef AzCreateDrawTargetForData(AzBackendType aBackend,
                                          unsigned char *aData,
                                          AzIntSize *aSize,
                                          int32_t aStride,
                                          AzSurfaceFormat aFormat);
AzDrawTargetRef
AzCreateDrawTargetSkiaWithGrContextAndFBO(SkiaGrContextRef aGrContext,
                                          unsigned int aFBOID,
                                          AzIntSize *aSize,
                                          AzSurfaceFormat aFormat);

void AzRetainDrawTarget(AzDrawTargetRef aTarget);

void AzReleaseDrawTarget(AzDrawTargetRef aTarget);

AzIntSize AzDrawTargetGetSize(AzDrawTargetRef aDrawTarget);
AzSurfaceFormat AzDrawTargetGetFormat(AzDrawTargetRef aDrawTarget);
void AzDrawTargetGetTransform(AzDrawTargetRef aDrawTarget, AzMatrix* aOutMatrix);
void AzDrawTargetFlush(AzDrawTargetRef aDrawTarget);
void AzDrawTargetClearRect(AzDrawTargetRef aDrawTarget, AzRect *aRect);
void AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget,
                          AzRect* aRect,
                          AzPatternRef aPattern,
                          AzDrawOptions* aDrawOptions);
void AzDrawTargetStroke(AzDrawTargetRef aDrawTarget,
                        AzPathRef aPath,
                        AzPatternRef aPattern,
                        const AzStrokeOptions *aStrokeOptions,
                        const AzDrawOptions *aDrawOptions);
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
void AzDrawTargetDrawFilter(AzDrawTargetRef aDrawTarget,
                            AzFilterNodeRef aFilter,
                            const AzRect *aSourceRect,
                            const AzPoint *aDestPoint,
                            const AzDrawOptions *aOptions);
void AzDrawTargetDrawSurfaceWithShadow(AzDrawTargetRef aDrawTarget,
                                       AzSourceSurfaceRef aSurface,
                                       const AzPoint* aPoint,
                                       const AzColor* aColor,
                                       const AzPoint* aOffset,
                                       AzFloat aSigma,
                                       AzCompositionOp aOperator);
AzSourceSurfaceRef AzDrawTargetGetSnapshot(AzDrawTargetRef aDrawTarget);
AzSourceSurfaceRef AzDrawTargetCreateSourceSurfaceFromData(AzDrawTargetRef aDrawTarget,
                                                           const unsigned char *aData,
                                                           AzIntSize *aSize,
                                                           int32_t aStride,
                                                           AzSurfaceFormat aFormat);
AzDrawTargetRef AzDrawTargetCreateShadowDrawTarget(AzDrawTargetRef aDrawTarget,
                                                   const AzIntSize *aSize,
                                                   AzSurfaceFormat aFormat,
                                                   AzFloat aSigma);
AzGradientStopsRef AzDrawTargetCreateGradientStops(AzDrawTargetRef aDrawTarget,
                                                   AzGradientStop *aStops,
                                                   uint32_t aNumStops,
                                                   AzExtendMode aExtendMode);
void AzDrawTargetPushClip(AzDrawTargetRef aDrawTarget, AzPathRef aPath);
void AzDrawTargetPushClipRect(AzDrawTargetRef aDrawTarget, const AzRect *aRect);
void AzDrawTargetPopClip(AzDrawTargetRef aDrawTarget);
AzFilterNodeRef AzDrawTargetCreateFilter(AzDrawTargetRef aDrawTarget, AzFilterType aType);

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
void AzPathBuilderBezierTo(AzPathBuilderRef aPathBuilder,
                           const AzPoint *aControlPoint1,
                           const AzPoint *aControlPoint2,
                           const AzPoint *aControlPoint3);
void AzPathBuilderQuadraticBezierTo(AzPathBuilderRef aPathBuilder,
                                    const AzPoint *aCP,
                                    const AzPoint *aEndPoint);
AzPoint AzPathBuilderCurrentPoint(AzPathBuilderRef aPathBuilder);
void AzPathBuilderClose(AzPathBuilderRef aPathBuilder);
AzPathRef AzPathBuilderFinish(AzPathBuilderRef aPathBuilder);

AzLinearGradientPatternRef AzCreateLinearGradientPattern(const AzPoint *aBegin,
                                                         const AzPoint *aEnd,
                                                         AzGradientStopsRef aStops,
                                                         const AzMatrix *aMatrix);

AzLinearGradientPatternRef AzCloneLinearGradientPattern(AzLinearGradientPatternRef aPattern);

AzRadialGradientPatternRef AzCreateRadialGradientPattern(const AzPoint *aCenter1,
                                                         const AzPoint *aCenter2,
                                                         AzFloat aRadius1,
                                                         AzFloat aRadius2,
                                                         AzGradientStopsRef aStops,
                                                         const AzMatrix *aMatrix);

AzRadialGradientPatternRef AzCloneRadialGradientPattern(AzRadialGradientPatternRef aPattern);

AzSurfacePatternRef AzCreateSurfacePattern(AzSourceSurfaceRef aSurface, AzExtendMode aExtendMode);

AzSurfacePatternRef AzCloneSurfacePattern(AzSurfacePatternRef aPattern);

AzIntSize AzSurfacePatternGetSize(AzSurfacePatternRef aPattern);

void AzReleasePattern(AzPatternRef aPattern);

void AzReleaseGradientStops(AzGradientStopsRef aStops);

/* Filters.h */

void AzReleaseFilterNode(AzFilterNodeRef aFilter);
void AzFilterNodeSetSourceSurfaceInput(AzFilterNodeRef aFilter,
                                       uint32_t aIndex,
                                       AzSourceSurfaceRef aSurface);
void AzFilterNodeSetFilterNodeInput(AzFilterNodeRef aFilter,
                                    uint32_t aIndex,
                                    AzFilterNodeRef aInputFilter);
void AzFilterNodeSetFloatAttribute(AzFilterNodeRef aFilter, uint32_t aIndex, AzFloat aValue);
void AzFilterNodeSetColorAttribute(AzFilterNodeRef aFilter,
                                   uint32_t aIndex,
                                   const AzColor *aValue);
void AzFilterNodeSetMatrix5x4Attribute(AzFilterNodeRef aFilter,
                                       uint32_t aIndex,
                                       const AzMatrix5x4 *aValue);
void AzFilterNodeSetBoolAttribute(AzFilterNodeRef aFilter, uint32_t aIndex, bool aValue);

/* Factory.h */

void AzReleaseScaledFont(AzScaledFontRef aFont);

/* Helpers */
typedef void AzFontOptions;
AzFontOptions* AzCreateFontOptionsForName(char *aName, AzFontStyle aStyle);
AzFontOptions* AzCreateFontOptionsForData(uint8_t *aFontData, uint32_t aFontDataSize);
void AzDestroyFontOptions(AzFontOptions* aOptions);

#ifdef __cplusplus
}
#endif

#endif /* MOZILLA_GFX_AZURE_C_H */

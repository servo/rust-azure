#ifndef MOZILLA_GFX_AZURE_C_H_
#define MOZILLA_GFX_AZURE_C_H_

#include <stdint.h>

#ifdef USE_CAIRO
#include <cairo/cairo.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// FIXME: Probably want to prefix all these with something
// FIXME: This stuff is copy pasted from the azure headers

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

/* 2D.h */

typedef void* AzDrawTargetRef;
typedef void* AzPatternRef;
typedef void* AzColorPatternRef;

void AzSanityCheck();

#ifdef USE_CAIRO
AzDrawTargetRef AzCreateDrawTargetForCairoSurface(cairo_surface_t* aSurface);
#endif

void AzReleaseDrawTarget(AzDrawTargetRef aTarget);

AzColorPatternRef AzCreateColorPattern(AzColor *aColor);

void AzReleaseColorPattern(AzColorPatternRef aColorPattern);

void AzDrawTargetFillRect(AzDrawTargetRef aDrawTarget, AzRect *aRect, AzPatternRef aPattern);

#ifdef __cplusplus
}
#endif

#endif /* MOZILLA_GFX_AZURE_C_H */

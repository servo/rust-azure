#ifndef MOZILLA_GFX_AZURE_C_H_
#define MOZILLA_GFX_AZURE_C_H_

#include <stdint.h>

// FIXME: Probably want to prefix all these with something
// FIXME: This stuff is copy pasted from the azure headers

/* Types.h */

typedef float Float;

enum SurfaceType
{
  SURFACE_DATA, /* Data surface - bitmap in memory */
  SURFACE_D2D1_BITMAP, /* Surface wrapping a ID2D1Bitmap */
  SURFACE_D2D1_DRAWTARGET, /* Surface made from a D2D draw target */
  SURFACE_CAIRO, /* Surface wrapping a cairo surface */
  SURFACE_CAIRO_IMAGE, /* Data surface wrapping a cairo image surface */
  SURFACE_COREGRAPHICS_IMAGE, /* Surface wrapping a CoreGraphics Image */
  SURFACE_COREGRAPHICS_CGCONTEXT, /* Surface wrapping a CG context */
  SURFACE_SKIA, /* Surface wrapping a Skia bitmap */
  SURFACE_DUAL_DT /* Snapshot of a dual drawtarget */
};


enum SurfaceFormat
{
  FORMAT_B8G8R8A8,
  FORMAT_B8G8R8X8,
  FORMAT_R5G6B5,
  FORMAT_A8
};

enum BackendType
{
  BACKEND_NONE,
  BACKEND_DIRECT2D,
  BACKEND_COREGRAPHICS,
  BACKEND_CAIRO,
  BACKEND_SKIA
};

enum FontType
{
  FONT_DWRITE,
  FONT_GDI,
  FONT_MAC,
  FONT_SKIA,
  FONT_CAIRO,
  FONT_COREGRAPHICS
};

enum NativeSurfaceType
{
  NATIVE_SURFACE_D3D10_TEXTURE,
  NATIVE_SURFACE_CAIRO_SURFACE,
  NATIVE_SURFACE_CGCONTEXT
};

enum NativeFontType
{
  NATIVE_FONT_DWRITE_FONT_FACE,
  NATIVE_FONT_GDI_FONT_FACE,
  NATIVE_FONT_MAC_FONT_FACE,
  NATIVE_FONT_SKIA_FONT_FACE,
  NATIVE_FONT_CAIRO_FONT_FACE
};

enum CompositionOp { OP_OVER, OP_ADD, OP_ATOP, OP_OUT, OP_IN, OP_SOURCE, OP_DEST_IN, OP_DEST_OUT, OP_DEST_OVER, OP_DEST_ATOP, OP_XOR, OP_COUNT };
enum ExtendMode { EXTEND_CLAMP, EXTEND_REPEAT, EXTEND_REFLECT };
enum FillRule { FILL_WINDING, FILL_EVEN_ODD };
enum AntialiasMode { AA_NONE, AA_GRAY, AA_SUBPIXEL };
enum Snapping { SNAP_NONE, SNAP_ALIGNED };
enum Filter { FILTER_LINEAR, FILTER_POINT };
enum PatternType { PATTERN_COLOR, PATTERN_SURFACE, PATTERN_LINEAR_GRADIENT, PATTERN_RADIAL_GRADIENT };
enum JoinStyle { JOIN_BEVEL, JOIN_ROUND, JOIN_MITER, JOIN_MITER_OR_BEVEL };
enum CapStyle { CAP_BUTT, CAP_ROUND, CAP_SQUARE };
enum SamplingBounds { SAMPLING_UNBOUNDED, SAMPLING_BOUNDED };

enum Side {eSideTop, eSideRight, eSideBottom, eSideLeft};

struct Color {
    Float r, g, b, a;
};

struct GradientStop {
    Float offset;
    Color color;
};

/* Rect.h */

struct IntRect {
    int32_t x, y, width, height;
};

struct Rect {
    Float x, y, width, height;
};

/* 2D.h */

typedef void* DrawTargetRef;
typedef void* PatternRef;
typedef void* ColorPatternRef;

#endif /* MOZILLA_GFX_AZURE_C_H */

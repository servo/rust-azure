#ifndef MOZILLA_GFX_AZURE_C_H_
#define MOZILLA_GFX_AZURE_C_H_

// FIXME: Probably want to prefix all these with something

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


#endif /* MOZILLA_GFX_AZURE_C_H */

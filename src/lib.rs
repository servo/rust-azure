/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#![crate_name = "azure"]
#![crate_type = "rlib"]

#![feature(globs)]

extern crate libc;
extern crate sync;
extern crate geom;
extern crate layers;
#[cfg(target_os = "linux")]
extern crate xlib;
#[cfg(target_os = "android")]
extern crate egl;
#[cfg(not(target_os = "macos"))]
extern crate freetype;
#[cfg(target_os = "macos")]
extern crate core_foundation;
#[cfg(target_os = "macos")]
extern crate core_graphics;
#[cfg(target_os = "macos")]
extern crate core_text;

pub use azure::{AzFontOptions, AzFloat, enum_AzSurfaceType, AZ_SURFACE_DATA, 
                AZ_SURFACE_D2D1_BITMAP, AZ_SURFACE_D2D1_DRAWTARGET, AZ_SURFACE_CAIRO, AZ_SURFACE_CAIRO_IMAGE, 
                AZ_SURFACE_COREGRAPHICS_IMAGE, AZ_SURFACE_COREGRAPHICS_CGCONTEXT, AZ_SURFACE_SKIA, AZ_SURFACE_DUAL_DT, 
                enum_AzSurfaceFormat, AZ_FORMAT_B8G8R8A8, AZ_FORMAT_B8G8R8X8, AZ_FORMAT_R5G6B5, 
                AZ_FORMAT_A8, AzSurfaceFormat, enum_AzBackendType, AZ_BACKEND_NONE, 
                AZ_BACKEND_DIRECT2D, AZ_BACKEND_COREGRAPHICS, AZ_BACKEND_COREGRAPHICS_ACCELERATED, AZ_BACKEND_CAIRO, 
                AZ_BACKEND_SKIA, AZ_BACKEND_RECORDING, AzBackendType, enum_AzFontType, 
                AZ_FONT_DWRITE, AZ_FONT_GDI, AZ_FONT_MAC, AZ_FONT_SKIA, 
                AZ_FONT_CAIRO, AZ_FONT_COREGRAPHICS, enum_AzNativeSurfaceType, AZ_NATIVE_SURFACE_D3D10_TEXTURE, 
                AZ_NATIVE_SURFACE_CAIRO_SURFACE, AZ_NATIVE_SURFACE_CGCONTEXT, enum_AzNativeFontType, AZ_NATIVE_FONT_DWRITE_FONT_FACE, 
                AZ_NATIVE_FONT_GDI_FONT_FACE, AZ_NATIVE_FONT_MAC_FONT_FACE, AZ_NATIVE_FONT_SKIA_FONT_FACE, AZ_NATIVE_FONT_CAIRO_FONT_FACE, 
                enum_AzFontStyle, AZ_FONT_STYLE_NORMAL, AZ_FONT_STYLE_ITALIC, AZ_FONT_STYLE_BOLD, 
                AZ_FONT_STYLE_BOLD_ITALIC, enum_AzCompositionOp, AZ_OP_OVER, AZ_OP_ADD, 
                AZ_OP_ATOP, AZ_OP_OUT, AZ_OP_IN, AZ_OP_SOURCE, 
                AZ_OP_DEST_IN, AZ_OP_DEST_OUT, AZ_OP_DEST_OVER, AZ_OP_DEST_ATOP, 
                AZ_OP_XOR, AZ_OP_MULTIPLY, AZ_OP_SCREEN, AZ_OP_OVERLAY, 
                AZ_OP_DARKEN, AZ_OP_LIGHTEN, AZ_OP_COLOR_DODGE, AZ_OP_COLOR_BURN, 
                AZ_OP_HARD_LIGHT, AZ_OP_SOFT_LIGHT, AZ_OP_DIFFERENCE, AZ_OP_EXCLUSION, 
                AZ_OP_HUE, AZ_OP_SATURATION, AZ_OP_COLOR, AZ_OP_LUMINOSITY, 
                AZ_OP_COUNT, enum_AzExtendMode, AZ_EXTEND_CLAMP, AZ_EXTEND_REPEAT, 
                AZ_EXTEND_REFLECT, enum_AzFillRule, AZ_FILL_WINDING, AZ_FILL_EVEN_ODD, 
                enum_AzAntialiasMode, AZ_AA_NONE, AZ_AA_GRAY, AZ_AA_SUBPIXEL, 
                enum_AzFilter, AZ_FILTER_LINEAR, AZ_FILTER_POINT, AzFilter, enum_AzPatternType,
                AZ_PATTERN_COLOR, AZ_PATTERN_SURFACE, AZ_PATTERN_LINEAR_GRADIENT, AZ_PATTERN_RADIAL_GRADIENT, 
                enum_AzJoinStyle, AZ_JOIN_BEVEL, AZ_JOIN_ROUND, AZ_JOIN_MITER, 
                AZ_JOIN_MITER_OR_BEVEL, enum_AzCapStyle, AZ_CAP_BUTT, AZ_CAP_ROUND, 
                AZ_CAP_SQUARE, enum_AzSamplingBounds, AZ_SAMPLING_UNBOUNDED, AZ_SAMPLING_BOUNDED, 
                enum_AzSide, AZ_eSideTop, AZ_eSideRight, AZ_eSideBottom, 
                AZ_eSideLeft, struct__AzColor, AzColor, struct__AzGradientStop, 
                AzGradientStop, struct__AzIntRect, AzIntRect, struct__AzRect, 
                AzRect, struct__AzIntPoint, AzIntPoint, struct__AzPoint, 
                AzPoint, struct__AzIntSize, AzIntSize, struct__AzSize, 
                AzSize, struct__AzMatrix, AzMatrix, struct__AzDrawOptions, 
                AzDrawOptions, struct__AzStrokeOptions, AzStrokeOptions, struct__AzDrawSurfaceOptions, 
                AzDrawSurfaceOptions, struct__AzGlyph, AzGlyph, struct__AzGlyphBuffer, 
                AzGlyphBuffer, struct__AzNativeFont, AzNativeFont, AzGradientStopsRef, 
                AzSkiaSharedGLContextRef, AzSkiaSharedGrGLContextRef, AzSkiaGrContextRef, AzSkiaGrGLSharedSurfaceRef, 
                AzDrawTargetRef, AzPatternRef, AzColorPatternRef, AzScaledFontRef, 
                AzGlyphRenderingOptionsRef, AzSourceSurfaceRef, AzDataSourceSurfaceRef, AzDrawSurfaceOptionsRef, 
                AzGLContext, AzSkiaGrGLSharedContextRef, AzGLContextMetadataRef, AzGLNativeContextRef, 
                AzPathRef, AzPathBuilderRef, AzSanityCheck, AzCreateColorPattern, 
                AzCreateSkiaSharedGLContext, AzRetainSkiaSharedGLContext, AzReleaseSkiaSharedGLContext, 
                AzSkiaSharedGLContextGetFBOID, AzSkiaSharedGLContextStealSurface, AzSkiaSharedGLContextGetGrContext, AzSkiaSharedGLContextMakeCurrent, 
                AzSkiaSharedGLContextFlush, AzCreateDrawTarget, AzCreateDrawTargetForData, AzCreateSkiaDrawTargetForFBO, 
                AzRetainDrawTarget, AzReleaseDrawTarget, AzDrawTargetGetSize, AzDrawTargetFlush, 
                AzDrawTargetClearRect, AzDrawTargetFillRect, AzDrawTargetStrokeRect, AzDrawTargetStrokeLine, 
                AzDrawTargetFill, AzDrawTargetPushClip, AzDrawTargetPopClip, AzDrawTargetFillGlyphs, 
                AzDrawTargetDrawSurface, AzDrawTargetGetSnapshot, AzDrawTargetCreateSourceSurfaceFromData, AzReleaseSourceSurface, 
                AzSourceSurfaceGetSize, AzSourceSurfaceGetFormat, AzSourceSurfaceGetDataSurface, AzDataSourceSurfaceGetData, 
                AzDataSourceSurfaceGetStride, AzCreateScaledFontForNativeFont, AzReleaseScaledFont, AzDrawTargetSetTransform, 
                AzCreateFontOptionsForName, AzDestroyFontOptions, AzSkiaGetCurrentGLContext, AzCreatePathBuilder, 
                AzReleasePathBuilder, AzPathBuilderMoveTo, AzPathBuilderLineTo, AzPathBuilderFinish, AzReleasePath};

pub mod azure_hl;
pub mod scaled_font;

#[cfg(target_os = "linux")]
pub mod test;

pub mod linkhack;

pub mod azure;

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#![crate_name = "azure"]
#![crate_type = "rlib"]
#![cfg_attr(feature = "plugins", feature(custom_derive, plugin, proc_macro, rustc_attrs))]
#![cfg_attr(feature = "plugins", plugin(heapsize_plugin))]

#[cfg(target_os = "macos")]
extern crate core_foundation;
#[cfg(target_os = "macos")]
extern crate core_graphics;
#[cfg(target_os = "macos")]
extern crate core_text;
#[cfg(target_os = "android")]
extern crate egl;
extern crate euclid;
#[cfg(not(target_os = "macos"))]
extern crate freetype;
#[macro_use]
#[cfg(feature = "plugins")]
extern crate heapsize;
extern crate libc;
#[cfg(feature = "plugins")]
extern crate serde;
#[cfg(feature = "plugins")]
#[macro_use]
extern crate serde_derive;
extern crate skia;
#[cfg(target_os = "linux")]
extern crate x11;

pub use azure::{AZ_AA_GRAY, AZ_AA_NONE, AZ_AA_SUBPIXEL, AZ_BACKEND_CAIRO, AZ_BACKEND_COREGRAPHICS,
                AZ_BACKEND_COREGRAPHICS_ACCELERATED, AZ_BACKEND_DIRECT2D, AZ_BACKEND_NONE,
                AZ_BACKEND_RECORDING, AZ_BACKEND_SKIA, AZ_CAP_BUTT, AZ_CAP_ROUND, AZ_CAP_SQUARE,
                AZ_EXTEND_CLAMP, AZ_EXTEND_REFLECT, AZ_EXTEND_REPEAT, AZ_FILL_EVEN_ODD,
                AZ_FILL_WINDING, AZ_FILTER_LINEAR, AZ_FILTER_POINT, AZ_FONT_CAIRO,
                AZ_FONT_COREGRAPHICS, AZ_FONT_DWRITE, AZ_FONT_GDI, AZ_FONT_MAC, AZ_FONT_SKIA,
                AZ_FONT_STYLE_BOLD, AZ_FONT_STYLE_BOLD_ITALIC, AZ_FONT_STYLE_ITALIC,
                AZ_FONT_STYLE_NORMAL, AZ_FORMAT_A8, AZ_FORMAT_B8G8R8A8, AZ_FORMAT_B8G8R8X8,
                AZ_FORMAT_R5G6B5, AZ_JOIN_BEVEL, AZ_JOIN_MITER, AZ_JOIN_MITER_OR_BEVEL,
                AZ_JOIN_ROUND, AZ_NATIVE_FONT_CAIRO_FONT_FACE, AZ_NATIVE_FONT_DWRITE_FONT_FACE,
                AZ_NATIVE_FONT_GDI_FONT_FACE, AZ_NATIVE_FONT_MAC_FONT_FACE,
                AZ_NATIVE_FONT_SKIA_FONT_FACE, AZ_NATIVE_SURFACE_CAIRO_SURFACE,
                AZ_NATIVE_SURFACE_CGCONTEXT, AZ_NATIVE_SURFACE_D3D10_TEXTURE, AZ_OP_ADD,
                AZ_OP_ATOP, AZ_OP_COLOR, AZ_OP_COLOR_BURN, AZ_OP_COLOR_DODGE, AZ_OP_COUNT,
                AZ_OP_DARKEN, AZ_OP_DEST_ATOP, AZ_OP_DEST_IN, AZ_OP_DEST_OUT, AZ_OP_DEST_OVER,
                AZ_OP_DIFFERENCE, AZ_OP_EXCLUSION, AZ_OP_HARD_LIGHT, AZ_OP_HUE, AZ_OP_IN,
                AZ_OP_LIGHTEN, AZ_OP_LUMINOSITY, AZ_OP_MULTIPLY, AZ_OP_OUT, AZ_OP_OVER,
                AZ_OP_OVERLAY, AZ_OP_SATURATION, AZ_OP_SCREEN, AZ_OP_SOFT_LIGHT, AZ_OP_SOURCE,
                AZ_OP_XOR, AZ_PATTERN_COLOR, AZ_PATTERN_LINEAR_GRADIENT,
                AZ_PATTERN_RADIAL_GRADIENT, AZ_PATTERN_SURFACE, AZ_SAMPLING_BOUNDED,
                AZ_SAMPLING_UNBOUNDED, AZ_SURFACE_CAIRO, AZ_SURFACE_CAIRO_IMAGE,
                AZ_SURFACE_COREGRAPHICS_CGCONTEXT, AZ_SURFACE_COREGRAPHICS_IMAGE,
                AZ_SURFACE_D2D1_BITMAP, AZ_SURFACE_D2D1_DRAWTARGET, AZ_SURFACE_DATA,
                AZ_SURFACE_DUAL_DT, AZ_SURFACE_SKIA, AZ_eSideBottom, AZ_eSideLeft, AZ_eSideRight,
                AZ_eSideTop, AzBackendType, AzColor, AzColorPatternRef, AzCreateColorPattern,
                AzCreateDrawTarget, AzCreateDrawTargetForData,
                AzCreateDrawTargetSkiaWithGrContextAndFBO, AzCreateFontOptionsForName,
                AzCreatePathBuilder, AzCreateScaledFontForNativeFont, AzDataSourceSurfaceGetData,
                AzDataSourceSurfaceGetStride, AzDataSourceSurfaceRef, AzDestroyFontOptions,
                AzDrawOptions, AzDrawSurfaceOptions, AzDrawSurfaceOptionsRef,
                AzDrawTargetClearRect, AzDrawTargetCopySurface,
                AzDrawTargetCreateSourceSurfaceFromData, AzDrawTargetDrawSurface,
                AzDrawTargetFill, AzDrawTargetFillGlyphs, AzDrawTargetFillRect, AzDrawTargetFlush,
                AzDrawTargetGetSize, AzDrawTargetGetSnapshot, AzDrawTargetPopClip,
                AzDrawTargetPushClip, AzDrawTargetRef, AzDrawTargetSetTransform,
                AzDrawTargetStroke, AzDrawTargetStrokeLine, AzDrawTargetStrokeRect, AzFilter,
                AzFloat, AzFontOptions, AzGLContext, AzGLContextMetadataRef, AzGlyph,
                AzGlyphBuffer, AzGlyphRenderingOptionsRef, AzGradientStop, AzGradientStopsRef,
                AzIntPoint, AzIntRect, AzIntSize, AzMatrix, AzNativeFont, AzPathBuilderFinish,
                AzPathBuilderLineTo, AzPathBuilderMoveTo, AzPathBuilderRef, AzPathContainsPoint,
                AzPathCopyToBuilder, AzPathRef, AzPatternRef, AzPoint, AzRect,
                AzReleaseDrawTarget, AzReleasePath, AzReleasePathBuilder, AzReleaseScaledFont,
                AzReleaseSourceSurface, AzRetainDrawTarget, AzSanityCheck, AzScaledFontRef,
                AzSize, AzSourceSurfaceGetDataSurface, AzSourceSurfaceGetFormat,
                AzSourceSurfaceGetSize, AzSourceSurfaceRef, AzStrokeOptions, AzSurfaceFormat,
                enum_AzAntialiasMode, enum_AzBackendType, enum_AzCapStyle, enum_AzCompositionOp,
                enum_AzExtendMode, enum_AzFillRule, enum_AzFilter, enum_AzFontStyle,
                enum_AzFontType, enum_AzJoinStyle, enum_AzNativeFontType,
                enum_AzNativeSurfaceType, enum_AzPatternType, enum_AzSamplingBounds, enum_AzSide,
                enum_AzSurfaceFormat, enum_AzSurfaceType, struct__AzColor, struct__AzDrawOptions,
                struct__AzDrawSurfaceOptions, struct__AzGlyph, struct__AzGlyphBuffer,
                struct__AzGradientStop, struct__AzIntPoint, struct__AzIntRect, struct__AzIntSize,
                struct__AzMatrix, struct__AzNativeFont, struct__AzPoint, struct__AzRect,
                struct__AzSize, struct__AzStrokeOptions };

pub mod azure;
pub mod azure_hl;
pub mod linkhack;
pub mod scaled_font;
#[cfg(target_os = "linux")]
pub mod test;

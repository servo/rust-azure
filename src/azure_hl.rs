// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

//! High-level bindings to Azure.

use azure::{AZ_CAP_BUTT, AZ_JOIN_MITER_OR_BEVEL, AZ_FILTER_TYPE_FLOOD};
use azure::{AZ_FILTER_TYPE_GAUSSIAN_BLUR, AZ_IN_FLOOD_IN, AZ_IN_GAUSSIAN_BLUR_IN};
use azure::{AZ_ATT_FLOOD_COLOR, AZ_ATT_GAUSSIAN_BLUR_STD_DEVIATION};
use azure::{AzPoint, AzRect, AzFloat, AzIntSize, AzColor, AzColorPatternRef, AzGradientStopsRef};
use azure::{AzStrokeOptions, AzDrawOptions, AzSurfaceFormat, AzFilter, AzDrawSurfaceOptions};
use azure::{AzBackendType, AzDrawTargetRef, AzSourceSurfaceRef, AzDataSourceSurfaceRef};
use azure::{AzScaledFontRef, AzGlyphRenderingOptionsRef, AzExtendMode, AzGradientStop};
use azure::{AzCompositionOp};
use azure::{struct__AzColor, struct__AzGlyphBuffer};
use azure::{struct__AzDrawOptions, struct__AzDrawSurfaceOptions, struct__AzIntSize};
use azure::{struct__AzPoint, struct__AzRect, struct__AzStrokeOptions};
use azure::{AzGLContext, AzSkiaSharedGLContextRef};
use azure::{AzCreateColorPattern, AzCreateDrawTarget, AzCreateDrawTargetForData};
use azure::{AzDataSourceSurfaceGetData, AzDataSourceSurfaceGetStride};
use azure::{AzDrawTargetClearRect};
use azure::{AzDrawTargetCreateSourceSurfaceFromData, AzCreateSkiaSharedGLContext};
use azure::{AzReleaseSkiaSharedGLContext, AzRetainSkiaSharedGLContext};
use azure::{AzDrawTargetDrawSurface, AzDrawTargetFillRect, AzDrawTargetFlush};
use azure::{AzDrawTargetGetSize, AzDrawTargetGetSnapshot, AzDrawTargetSetTransform};
use azure::{AzDrawTargetStrokeLine, AzDrawTargetStrokeRect, AzDrawTargetFillGlyphs};
use azure::{AzDrawTargetCreateGradientStops, AzDrawTargetGetFormat};
use azure::{AzReleaseDrawTarget, AzReleasePattern, AzReleaseGradientStops};
use azure::{AzReleaseSourceSurface, AzRetainDrawTarget};
use azure::{AzSourceSurfaceGetDataSurface, AzSourceSurfaceGetFormat};
use azure::{AzSourceSurfaceGetSize, AzCreateSkiaDrawTargetForFBO, AzSkiaGetCurrentGLContext};
use azure::{AzSkiaSharedGLContextMakeCurrent, AzSkiaSharedGLContextStealSurface};
use azure::{AzSkiaSharedGLContextFlush, AzSkiaGrGLSharedSurfaceRef};
use azure::{AzCreatePathBuilder, AzPathBuilderRef, AzPathBuilderMoveTo, AzPathBuilderLineTo};
use azure::{AzPathBuilderArc, AzPathBuilderFinish, AzReleasePathBuilder};
use azure::{AzDrawTargetFill, AzPathRef, AzReleasePath, AzDrawTargetPushClip, AzDrawTargetPopClip};
use azure::{AzGLNativeContextRef, AzLinearGradientPatternRef, AzMatrix, AzPatternRef};
use azure::{AzCreateLinearGradientPattern, AzDrawTargetPushClipRect};
use azure::{AzDrawTargetDrawSurfaceWithShadow, AzDrawTargetCreateShadowDrawTarget};
use azure::{AzDrawTargetCreateSimilarDrawTarget, AzDrawTargetGetTransform};
use azure::{AzFilterNodeSetSourceSurfaceInput, AzReleaseFilterNode, AzDrawTargetCreateFilter};
use azure::{AzFilterNodeSetColorAttribute, AzFilterNodeSetFloatAttribute};
use azure::{AzFilterNodeSetFilterNodeInput, AzDrawTargetDrawFilter, AzFilterNodeRef, AzFilterType};

use sync::Arc;
use geom::matrix2d::Matrix2D;
use geom::point::Point2D;
use geom::rect::Rect;
use geom::size::Size2D;
use layers::platform::surface::NativePaintingGraphicsContext;
use libc::types::common::c99::{uint8_t, uint16_t};
use libc::size_t;
use std::mem;
use std::ptr;
use std::slice;

pub trait AsAzureRect {
    fn as_azure_rect(&self) -> AzRect;
}

impl AsAzureRect for Rect<AzFloat> {
    fn as_azure_rect(&self) -> AzRect {
        struct__AzRect {
            x: self.origin.x,
            y: self.origin.y,
            width: self.size.width,
            height: self.size.height
        }
    }
}

pub trait AsAzureIntSize {
    fn as_azure_int_size(&self) -> AzIntSize;
}

impl AsAzureIntSize for Size2D<i32> {
    fn as_azure_int_size(&self) -> AzIntSize {
        struct__AzIntSize {
            width: self.width,
            height: self.height
        }
    }
}

pub trait AsAzurePoint {
    fn as_azure_point(&self) -> AzPoint;
}

impl AsAzurePoint for Point2D<AzFloat> {
    fn as_azure_point(&self) -> AzPoint {
        struct__AzPoint {
            x: self.x,
            y: self.y
        }
    }
}

#[deriving(Clone)]
pub struct Color {
    pub r: AzFloat,
    pub g: AzFloat,
    pub b: AzFloat,
    pub a: AzFloat,
}

impl Color {
    pub fn new(r: AzFloat, g: AzFloat, b: AzFloat, a: AzFloat) -> Color {
        Color { r: r, g: g, b: b, a: a }
    }

    fn as_azure_color(&self) -> AzColor {
        struct__AzColor { r: self.r, g: self.g, b: self.b, a: self.a }
    }
}


// FIXME: Should have a class hierarchy here starting with Pattern.
pub struct ColorPattern {
    pub azure_color_pattern: AzColorPatternRef,
}

impl Drop for ColorPattern {
    fn drop(&mut self) {
        unsafe {
            AzReleasePattern(self.azure_color_pattern);
        }
    }
}

impl ColorPattern {
    pub fn new(color: Color) -> ColorPattern {
        unsafe {
            ColorPattern {
                azure_color_pattern: AzCreateColorPattern(&mut color.as_azure_color())
            }
        }
    }
}

pub enum CompositionOp {
    Over,
    Add,
    Atop,
    Out,
    In,
    Source,
    DestIn,
    DestOut,
    DestOver,
    DestAtop,
    Xor,
    Multiply,
    Screen,
    Overlay,
    Darken,
    Lighten,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
    Count,
}

#[allow(non_snake_case)]
pub struct StrokeOptions {
    pub line_width: AzFloat,
    pub miter_limit: AzFloat,
    pub mDashPattern: *mut AzFloat,
    pub mDashLength: size_t,
    pub fields: uint8_t
}

impl StrokeOptions {
    pub fn new(line_width: AzFloat, miter_limit: AzFloat) -> StrokeOptions {
        StrokeOptions {
            line_width: line_width,
            miter_limit: miter_limit,
            mDashPattern: ptr::null_mut(),
            mDashLength: 0,
            fields: AZ_CAP_BUTT as u8 << 4 | AZ_JOIN_MITER_OR_BEVEL as u8
        }
    }

    fn as_azure_stroke_options(&self) -> AzStrokeOptions {
        struct__AzStrokeOptions {
            mLineWidth: self.line_width,
            mMiterLimit: self.miter_limit,
            mDashPattern: self.mDashPattern,
            mDashLength: self.mDashLength,
            mDashOffset: 0.0 as AzFloat,
            fields: self.fields
        }
    }

    pub fn set_join_style(&mut self, style: u8) {
        self.fields = self.fields & 0b1111_0000_u8;
        self.fields = self.fields | style ;
    }

    pub fn set_cap_style(&mut self, style: u8) {
        self.fields = self.fields & 0b0000_1111_u8;
        self.fields = self.fields | (style << 4);
    }
}

pub struct DrawOptions {
    pub alpha: AzFloat,
    pub fields: uint16_t,
}

impl DrawOptions {
    pub fn new(alpha: AzFloat, fields: uint16_t) -> DrawOptions {
        DrawOptions {
            alpha : alpha,
            fields : fields,
        }
    }

    fn as_azure_draw_options(&self) -> AzDrawOptions {
        struct__AzDrawOptions {
            mAlpha: self.alpha,
            fields: self.fields
        }
    }

    pub fn set_composition_op(&mut self, style: CompositionOp) {
        self.fields = self.fields & 0b1111_1111_0000_0000_u16;
        self.fields = self.fields | (style as u16);
    }

    pub fn set_antialias_mode(&mut self, style: u8) {
        self.fields = self.fields & 0b1111_1000_1111_1111_u16; 
        let style = ((style & 7) as u16) << 8;
        self.fields = self.fields | style;
    }
}


pub enum SurfaceFormat {
    B8G8R8A8,
    B8G8R8X8,
    R8G8B8A8,
    R8G8B8X8,
    R5G6B5,
    A8,
    YUV,
    UNKNOWN
}

impl SurfaceFormat {
    fn as_azure_surface_format(self) -> AzSurfaceFormat {
        self as AzSurfaceFormat
    }

    pub fn new(azure_surface_format: AzSurfaceFormat) -> SurfaceFormat {
        match azure_surface_format {
            0 => SurfaceFormat::B8G8R8A8,
            1 => SurfaceFormat::B8G8R8X8,
            2 => SurfaceFormat::R8G8B8A8,
            3 => SurfaceFormat::R8G8B8X8,
            4 => SurfaceFormat::R5G6B5,
            5 => SurfaceFormat::A8,
            6 => SurfaceFormat::YUV,
            7 => SurfaceFormat::UNKNOWN,
            _ => panic!("SurfaceFormat::new(): unknown Azure surface format")
        }
    }
}

pub enum Filter {
    Good,
    Linear,
    Point
}

impl Filter {
    pub fn as_azure_filter(self) -> AzFilter {
        self as AzFilter
    }
}

pub struct DrawSurfaceOptions {
    pub filter: Filter,
    pub sampling_bounds: bool,
}

impl DrawSurfaceOptions {
    pub fn new(filter: Filter, sampling_bounds: bool) -> DrawSurfaceOptions {
        DrawSurfaceOptions {
            filter: filter,
            sampling_bounds: sampling_bounds,
        }
    }

    fn as_azure_draw_surface_options(&self) -> AzDrawSurfaceOptions {
        struct__AzDrawSurfaceOptions {
            filter: self.filter as i8,
            sampling_bounds: self.sampling_bounds as i8,
        }
    }
}


#[deriving(Clone, PartialEq)]
pub enum BackendType {
    None,
    Direct2D,
    CoreGraphics,
    CoreGraphicsAccelerated,
    Cairo,
    Skia,
    Recording,
    Direct2D11,
    NVPathRendering,
}

impl BackendType {
    pub fn as_azure_backend_type(self) -> AzBackendType {
        match self {
            BackendType::None                    => 0,
            BackendType::Direct2D                => 1,
            BackendType::CoreGraphics            => 2,
            BackendType::CoreGraphicsAccelerated => 3,
            BackendType::Cairo                   => 4,
            BackendType::Skia                    => 5,
            BackendType::Recording               => 6,
            BackendType::Direct2D11              => 7,
            BackendType::NVPathRendering         => 8,
        }
    }
}

pub struct DrawTarget {
    pub azure_draw_target: AzDrawTargetRef,
    pub data: Option<Arc<Vec<u8>>>,
    pub skia_context: Option<AzSkiaSharedGLContextRef>
}

impl Drop for DrawTarget {
    fn drop(&mut self) {
        unsafe {
            AzReleaseDrawTarget(self.azure_draw_target);
            match self.skia_context {
                None => {}
                Some(ctx_ref) => { AzReleaseSkiaSharedGLContext(ctx_ref); }
            }
        }
    }
}

impl PartialEq for DrawTarget {
    #[inline]
    fn eq(&self, other: &DrawTarget) -> bool {
        self.azure_draw_target == other.azure_draw_target
    }
}

/// Contains the GL resources that Skia was holding onto that may be safely extracted. At the
/// moment this consists simply of the native surface.
pub struct StolenGLResources {
    pub surface: AzSkiaGrGLSharedSurfaceRef,
}

impl DrawTarget {
    pub fn new(backend: BackendType, size: Size2D<i32>, format: SurfaceFormat)
                   -> DrawTarget {
        let azure_draw_target = unsafe {
            AzCreateDrawTarget(backend.as_azure_backend_type(),
                                                       &mut size.as_azure_int_size(),
                                                       format.as_azure_surface_format())
        };
        if azure_draw_target.is_null() {
            panic!("null azure draw target");
        }
        DrawTarget {
            azure_draw_target: azure_draw_target,
            data: None,
            skia_context: None
        }
    }

    pub fn new_with_data(backend: BackendType,
                         mut data: Vec<u8>,
                         offset: uint,
                         size: Size2D<i32>,
                         stride: i32,
                         format: SurfaceFormat) -> DrawTarget {
        assert!((data.len() - offset) as i32 >= stride * size.height);
        let azure_draw_target = unsafe {
            AzCreateDrawTargetForData(backend.as_azure_backend_type(),
                                      &mut data[offset],
                                      &mut size.as_azure_int_size(),
                                      stride,
                                      format.as_azure_surface_format())
        };
        if azure_draw_target.is_null() {
            panic!("null azure draw target");
        }
        DrawTarget {
            azure_draw_target: azure_draw_target,
            data: Some(Arc::new(data)),
            skia_context: None
        }
    }

    pub fn new_with_fbo(backend: BackendType,
                        native_graphics_context: &NativePaintingGraphicsContext,
                        size: Size2D<i32>,
                        format: SurfaceFormat) -> DrawTarget {
        assert!(backend == BackendType::Skia);
        let native_graphics_context = native_graphics_context as *const _ as AzGLNativeContextRef;
        let skia_context = unsafe {
            AzCreateSkiaSharedGLContext(native_graphics_context,
                                        &mut size.as_azure_int_size())
        };
        let azure_draw_target = unsafe {
            AzCreateSkiaDrawTargetForFBO(skia_context,
                                         &mut size.as_azure_int_size(),
                                         format.as_azure_surface_format())
        };
        if azure_draw_target.is_null() {
            panic!("null azure draw target");
        }
        DrawTarget {
            azure_draw_target: azure_draw_target,
            data: None,
            skia_context: Some(skia_context)
        }
    }

    pub fn clone(&self) -> DrawTarget {
        unsafe {
            AzRetainDrawTarget(self.azure_draw_target);
            match self.skia_context {
                Some(ctx) => AzRetainSkiaSharedGLContext(ctx),
                None => (),
            }
        }
        DrawTarget {
            azure_draw_target: self.azure_draw_target,
            data: match self.data {
                None => None,
                Some(ref arc) => Some(arc.clone())
            },
            skia_context: self.skia_context
        }
    }

    pub fn make_current(&self) {
        match self.skia_context {
            None => {}
            Some(ctx) => { 
                unsafe {
                    AzSkiaSharedGLContextMakeCurrent(ctx);
                }
            }
        }
    }

    /// Consumes this draw target and returns the underlying native surface and GL context, if they exist.
    pub fn steal_gl_resources(self) -> Option<StolenGLResources> {
        self.skia_context.map(|ctx| {
            StolenGLResources {
                surface: unsafe { AzSkiaSharedGLContextStealSurface(ctx) },
            }
        })
    }

    pub fn get_size(&self) -> AzIntSize {
        unsafe {
            AzDrawTargetGetSize(self.azure_draw_target)
        }
    }

    pub fn get_format(&self) -> SurfaceFormat {
        unsafe {
            SurfaceFormat::new(AzDrawTargetGetFormat(self.azure_draw_target))
        }
    }

    pub fn get_transform(&self) -> Matrix2D<AzFloat> {
        unsafe {
            let mut result: AzMatrix = mem::uninitialized();
            AzDrawTargetGetTransform(self.azure_draw_target, &mut result);
            mem::transmute::<AzMatrix,Matrix2D<AzFloat>>(result)
        }
    }

    pub fn flush(&self) {
        unsafe {
            AzDrawTargetFlush(self.azure_draw_target);
            match self.skia_context {
                None => {}
                Some(ctx) => { AzSkiaSharedGLContextFlush(ctx); }
            }
        }
    }

    pub fn clear_rect(&self, rect: &Rect<AzFloat>) {
        unsafe {
            AzDrawTargetClearRect(self.azure_draw_target, &mut rect.as_azure_rect());
        }
    }

    pub fn fill(&self, path: &Path, pattern: &ColorPattern, draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetFill(self.azure_draw_target,
                             path.azure_path,
                             pattern.azure_color_pattern,
                             &mut draw_options.as_azure_draw_options());
        }
    }

    pub fn fill_rect(&self,
                     rect: &Rect<AzFloat>,
                     pattern: PatternRef,
                     draw_options: Option<&DrawOptions>) {
        let mut draw_options = draw_options.map(|draw_options| {
            draw_options.as_azure_draw_options()
        });
        let draw_options = match draw_options {
            None => ptr::null_mut(),
            Some(ref mut draw_options) => draw_options as *mut AzDrawOptions
        };
        unsafe {
            AzDrawTargetFillRect(self.azure_draw_target,
                                 &mut rect.as_azure_rect(),
                                 pattern.as_azure_pattern(),
                                 draw_options);
        }
    }

    pub fn stroke_line(&self,
                   start: Point2D<AzFloat>,
                   end: Point2D<AzFloat>,
                   pattern: &ColorPattern,
                   stroke_options: &StrokeOptions,
                   draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetStrokeLine(self.azure_draw_target,
                                   &mut start.as_azure_point(),
                                   &mut end.as_azure_point(),
                                   pattern.azure_color_pattern,
                                   &mut stroke_options.as_azure_stroke_options(),
                                   &mut draw_options.as_azure_draw_options());
        }
    }

    pub fn stroke_rect(&self,
                   rect: &Rect<AzFloat>,
                   pattern: &ColorPattern,
                   stroke_options: &StrokeOptions,
                   draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetStrokeRect(self.azure_draw_target,
                                   &mut rect.as_azure_rect(),
                                   pattern.azure_color_pattern,
                                   &mut stroke_options.as_azure_stroke_options(),
                                   &mut draw_options.as_azure_draw_options());
        }
    }

    pub fn draw_surface(&self,
                        surface: SourceSurface,
                        dest: Rect<AzFloat>,
                        source: Rect<AzFloat>,
                        surf_options: DrawSurfaceOptions,
                        options: DrawOptions) {
        unsafe {
            AzDrawTargetDrawSurface(self.azure_draw_target,
                                    surface.azure_source_surface,
                                    &mut dest.as_azure_rect(),
                                    &mut source.as_azure_rect(),
                                    &mut surf_options.as_azure_draw_surface_options(),
                                    &mut options.as_azure_draw_options());
        }
    }

    pub fn draw_filter(&self,
                       filter: FilterNode,
                       source_rect: &Rect<AzFloat>,
                       dest_point: &Point2D<AzFloat>,
                       options: DrawOptions) {
        unsafe {
            AzDrawTargetDrawFilter(self.azure_draw_target,
                                   filter.azure_filter_node,
                                   mem::transmute::<_,*const AzRect>(source_rect),
                                   mem::transmute::<_,*const AzPoint>(dest_point),
                                   &options.as_azure_draw_options())
        }
    }

    pub fn draw_surface_with_shadow(&self,
                                    surface: SourceSurface,
                                    dest: &Point2D<AzFloat>,
                                    color: &Color,
                                    offset: &Point2D<AzFloat>,
                                    sigma: AzFloat,
                                    operator: CompositionOp) {
        unsafe {
            AzDrawTargetDrawSurfaceWithShadow(self.azure_draw_target,
                                              surface.azure_source_surface,
                                              mem::transmute::<_,*const AzPoint>(dest),
                                              mem::transmute::<_,*const AzColor>(color),
                                              mem::transmute::<_,*const AzPoint>(offset),
                                              sigma,
                                              operator as AzCompositionOp)
        }
    }

    pub fn snapshot(&self) -> SourceSurface {
        unsafe {
            let azure_surface = AzDrawTargetGetSnapshot(self.azure_draw_target);
            SourceSurface::new(azure_surface)
        }
    }

    pub fn create_source_surface_from_data(&self,
                                           data: &[u8],
                                           size: Size2D<i32>,
                                           stride: i32,
                                           format: SurfaceFormat)
                                           -> SourceSurface {
        assert!(data.len() as i32 == stride * size.height);
        unsafe {
            let azure_surface = AzDrawTargetCreateSourceSurfaceFromData(
                self.azure_draw_target,
                &data[0],
                &mut size.as_azure_int_size(),
                stride,
                format.as_azure_surface_format());
            SourceSurface::new(azure_surface)
        }
    }

    pub fn create_similar_draw_target(&self, size: &Size2D<i32>, format: SurfaceFormat)
                                      -> DrawTarget {
        unsafe {
            let new_draw_target = AzDrawTargetCreateSimilarDrawTarget(
                self.azure_draw_target,
                &size.as_azure_int_size(),
                format.as_azure_surface_format());
            DrawTarget {
                azure_draw_target: new_draw_target,
                data: None,
                skia_context: None,
            }
        }
    }

    pub fn create_shadow_draw_target(&self,
                                     size: &Size2D<i32>,
                                     format: SurfaceFormat,
                                     sigma: AzFloat)
                                     -> DrawTarget {
        unsafe {
            let new_draw_target = AzDrawTargetCreateShadowDrawTarget(
                self.azure_draw_target,
                &size.as_azure_int_size(),
                format.as_azure_surface_format(),
                sigma);
            DrawTarget {
                azure_draw_target: new_draw_target,
                data: None,
                skia_context: None,
            }
        }
    }

    pub fn create_gradient_stops(&self,
                                 gradient_stops: &[GradientStop],
                                 extend_mode: ExtendMode)
                                 -> GradientStops {
        unsafe {
            GradientStops::new(AzDrawTargetCreateGradientStops(
                    self.azure_draw_target,
                    mem::transmute::<_,*const AzGradientStop>(&gradient_stops[0]),
                    gradient_stops.len() as u32,
                    extend_mode.as_azure_extend_mode()))
        }
    }

    pub fn set_transform(&self, matrix: &Matrix2D<AzFloat>) {
        unsafe {
            AzDrawTargetSetTransform(self.azure_draw_target, mem::transmute(matrix));
        }
    }

    pub fn fill_glyphs(&self,
                       azfontref: AzScaledFontRef,
                       mut glyphbuf: struct__AzGlyphBuffer,
                       azure_pattern: AzColorPatternRef,
                       mut options: struct__AzDrawOptions,
                       rendering_options: AzGlyphRenderingOptionsRef) {
        unsafe {
            AzDrawTargetFillGlyphs(self.azure_draw_target,
                    azfontref,
                    &mut glyphbuf,
                    azure_pattern,
                    &mut options,
                    rendering_options);
        }
    }

    pub fn create_path_builder(&self) -> PathBuilder {
        unsafe {
            PathBuilder {
                azure_path_builder: AzCreatePathBuilder(self.azure_draw_target)
            }
        }
    }

    pub fn create_filter(&self, filter_type: FilterType) -> FilterNode {
        unsafe {
            FilterNode {
                azure_filter_node: AzDrawTargetCreateFilter(self.azure_draw_target,
                                                            filter_type.as_azure_filter_type()),
            }
        }
    }

    pub fn push_clip(&self, path: &Path) {
        unsafe {
            AzDrawTargetPushClip(self.azure_draw_target,path.azure_path);
        }
    }

    pub fn push_clip_rect(&self, rect: &Rect<AzFloat>) {
        unsafe {
            AzDrawTargetPushClipRect(self.azure_draw_target, &rect.as_azure_rect());
        }
    }

    pub fn pop_clip(&self) {
        unsafe {
            AzDrawTargetPopClip(self.azure_draw_target);
        }
    }
}

// Ugly workaround for the lack of explicit self.
pub fn clone_mutable_draw_target(draw_target: &mut DrawTarget) -> DrawTarget {
    return draw_target.clone();
}

pub struct SourceSurface {
    pub azure_source_surface: AzSourceSurfaceRef,
}

impl Drop for SourceSurface {
    fn drop(&mut self) {
        unsafe {
            AzReleaseSourceSurface(self.azure_source_surface);
        }
    }
}

impl SourceSurface {
    pub fn new(azure_source_surface: AzSourceSurfaceRef) -> SourceSurface {
        SourceSurface {
            azure_source_surface: azure_source_surface
        }
    }
}

pub struct GradientStops {
    pub azure_gradient_stops: AzGradientStopsRef,
}

impl Drop for GradientStops {
    fn drop(&mut self) {
        unsafe {
            AzReleaseGradientStops(self.azure_gradient_stops);
        }
    }
}

impl GradientStops {
    pub fn new(azure_gradient_stops: AzGradientStopsRef) -> GradientStops {
        GradientStops {
            azure_gradient_stops: azure_gradient_stops,
        }
    }
}

#[repr(C)]
#[deriving(Clone)]
pub struct GradientStop {
    pub offset: AzFloat,
    pub color: Color,
}

#[repr(i32)]
#[deriving(Clone, PartialEq)]
pub enum ExtendMode {
    Clamp = 0,
    Repeat = 1,
    Reflect = 2,
}

impl ExtendMode {
    fn as_azure_extend_mode(self) -> AzExtendMode {
        self as AzExtendMode
    }
}

// FIXME Rust #8753 no fixed stack segment for default methods
#[allow(non_snake_case)]
unsafe fn AzSourceSurfaceGetSize_(aSurface: AzSourceSurfaceRef) -> AzIntSize {
    AzSourceSurfaceGetSize(aSurface)
}

// FIXME Rust #8753 no fixed stack segment for default methods
#[allow(non_snake_case)]
unsafe fn AzSourceSurfaceGetFormat_(aSurface: AzSourceSurfaceRef) -> AzSurfaceFormat {
    AzSourceSurfaceGetFormat(aSurface)
}

pub trait SourceSurfaceMethods {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef;

    fn size(&self) -> Size2D<i32> {
        let size = unsafe {
            AzSourceSurfaceGetSize_(self.get_azure_source_surface())
        };
        Size2D { width: size.width, height: size.height }
    }

    fn format(&self) -> SurfaceFormat {
        unsafe {
            SurfaceFormat::new(AzSourceSurfaceGetFormat_(self.get_azure_source_surface()))
        }
    }
}

impl SourceSurface {
    pub fn get_data_surface(&self) -> DataSourceSurface {
        let data_source_surface = unsafe {
            AzSourceSurfaceGetDataSurface(self.azure_source_surface)
        };
        DataSourceSurface {
            azure_data_source_surface: data_source_surface
        }
    }
}

impl SourceSurfaceMethods for SourceSurface {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef { self.azure_source_surface }
}

pub struct DataSourceSurface {
    pub azure_data_source_surface: AzDataSourceSurfaceRef,
}

impl Drop for DataSourceSurface {
    fn drop(&mut self) {
        unsafe {
            AzReleaseSourceSurface(self.azure_data_source_surface);
        }
    }
}

impl DataSourceSurface {
    pub fn with_data(&self, f: |&[u8]|) {
        unsafe {
            let buf = AzDataSourceSurfaceGetData(self.azure_data_source_surface) as *const u8;
            let len = self.stride() * self.size().height;
            slice::raw::buf_as_slice(buf, len as uint, f);
        }
    }

    pub fn stride(&self) -> i32 {
        unsafe {
            AzDataSourceSurfaceGetStride(self.azure_data_source_surface)
        }
    }

    // FIXME: Workaround for lack of working cross-crate default methods.
    pub fn get_size(&self) -> Size2D<i32> {
        self.size()
    }
}

impl SourceSurfaceMethods for DataSourceSurface {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef {
        self.azure_data_source_surface
    }
}

pub struct Path {
    pub azure_path: AzPathRef
}

impl Drop for Path {
    fn drop(&mut self) {
        unsafe {
            AzReleasePath(self.azure_path);
        }
    }
}

pub struct PathBuilder {
    pub azure_path_builder: AzPathBuilderRef
}

impl PathBuilder {
    pub fn move_to(&self, point: Point2D<AzFloat>) {
        let mut az_point = point.as_azure_point();
        unsafe {
            AzPathBuilderMoveTo(self.azure_path_builder, &mut az_point);
        }
    }

    pub fn line_to(&self, point: Point2D<AzFloat>) {
        let mut az_point = point.as_azure_point();
        unsafe {
            AzPathBuilderLineTo(self.azure_path_builder, &mut az_point);
        }
    }

    /// Adds an arc to the current figure.
    pub fn arc(&self,
               origin: Point2D<AzFloat>,
               radius: AzFloat,
               start_angle: AzFloat,
               end_angle: AzFloat,
               anticlockwise: bool) {
        let origin = origin.as_azure_point();
        unsafe {
            AzPathBuilderArc(self.azure_path_builder,
                             &origin,
                             radius,
                             start_angle,
                             end_angle,
                             anticlockwise)
        }
    }

    pub fn finish(&self) -> Path {
        let az_path = unsafe { AzPathBuilderFinish(self.azure_path_builder) };
        Path {
            azure_path : az_path
        }
    }
}

impl Drop for PathBuilder {
    fn drop(&mut self) {
        unsafe {
            AzReleasePathBuilder(self.azure_path_builder);
        }
    }
}

pub struct LinearGradientPattern {
    pub azure_linear_gradient_pattern: AzLinearGradientPatternRef,
}

impl Drop for LinearGradientPattern {
    fn drop(&mut self) {
        unsafe {
            AzReleasePattern(self.azure_linear_gradient_pattern);
        }
    }
}

impl LinearGradientPattern {
    pub fn new(begin: &Point2D<AzFloat>,
               end: &Point2D<AzFloat>,
               stops: GradientStops,
               matrix: &Matrix2D<AzFloat>)
               -> LinearGradientPattern {
        unsafe {
            LinearGradientPattern {
                azure_linear_gradient_pattern:
                    AzCreateLinearGradientPattern(mem::transmute::<_,*const AzPoint>(begin),
                                                  mem::transmute::<_,*const AzPoint>(end),
                                                  stops.azure_gradient_stops,
                                                  mem::transmute::<_,*const AzMatrix>(matrix)),
            }
        }
    }
}

pub enum PatternRef<'a> {
    Color(&'a ColorPattern),
    LinearGradient(&'a LinearGradientPattern),
}

impl<'a> PatternRef<'a> {
    fn as_azure_pattern(&self) -> AzPatternRef {
        match *self {
            PatternRef::Color(color_pattern) => {
                color_pattern.azure_color_pattern
            },
            PatternRef::LinearGradient(linear_gradient_pattern) => {
                linear_gradient_pattern.azure_linear_gradient_pattern
            }
        }
    }
}

pub fn current_gl_context() -> AzGLContext {
    unsafe {
        AzSkiaGetCurrentGLContext()
    }
}

pub struct FilterNode {
    pub azure_filter_node: AzFilterNodeRef,
}

impl Drop for FilterNode {
    fn drop(&mut self) {
        unsafe {
            AzReleaseFilterNode(self.azure_filter_node);
        }
    }
}

impl FilterNode {
    pub fn set_input<FIndex,FInput>(&self, index: FIndex, input: &FInput)
                                    where FIndex: FilterInputIndex, FInput: FilterInput {
        input.set(self, index.azure_index())
    }
    pub fn set_attribute<A>(&self, attribute: A) where A: FilterAttribute {
        attribute.set(self)
    }
}

pub struct FloodFilterInput;

pub struct GaussianBlurInput;

pub trait FilterInputIndex {
    fn azure_index(&self) -> u32;
}

impl FilterInputIndex for FloodFilterInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_FLOOD_IN
    }
}

impl FilterInputIndex for GaussianBlurInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_GAUSSIAN_BLUR_IN
    }
}

pub trait FilterAttribute {
    fn set(&self, filter_node: &FilterNode);
}

pub enum FloodAttribute {
    ColorFloodAttribute(Color),
}

pub enum GaussianBlurAttribute {
    StdDeviationGaussianBlurAttribute(AzFloat),
}

impl FilterAttribute for FloodAttribute {
    fn set(&self, filter_node: &FilterNode) {
        let FloodAttribute::ColorFloodAttribute(value) = *self;
        unsafe {
            AzFilterNodeSetColorAttribute(filter_node.azure_filter_node,
                                          AZ_ATT_FLOOD_COLOR,
                                          &value.as_azure_color())
        }
    }
}

impl FilterAttribute for GaussianBlurAttribute {
    fn set(&self, filter_node: &FilterNode) {
        let GaussianBlurAttribute::StdDeviationGaussianBlurAttribute(value) = *self;
        unsafe {
            AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                          AZ_ATT_GAUSSIAN_BLUR_STD_DEVIATION,
                                          value)
        }
    }
}

pub enum FilterType {
    FloodFilterType,
    GaussianBlurFilterType,
}

impl FilterType {
    pub fn as_azure_filter_type(self) -> AzFilterType {
        match self {
            FilterType::FloodFilterType => AZ_FILTER_TYPE_FLOOD,
            FilterType::GaussianBlurFilterType => AZ_FILTER_TYPE_GAUSSIAN_BLUR,
        }
    }
}

pub trait FilterInput {
    fn set(&self, filter: &FilterNode, index: u32);
}

impl FilterInput for SourceSurface {
    fn set(&self, filter: &FilterNode, index: u32) {
        unsafe {
            AzFilterNodeSetSourceSurfaceInput(filter.azure_filter_node,
                                              index,
                                              self.azure_source_surface)
        }
    }
}

impl FilterInput for FilterNode {
    fn set(&self, filter: &FilterNode, index: u32) {
        unsafe {
            AzFilterNodeSetFilterNodeInput(filter.azure_filter_node, index, self.azure_filter_node)
        }
    }
}


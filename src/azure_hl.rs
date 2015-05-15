// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

//! High-level bindings to Azure.

pub use AzColor as Color;
use azure::{AZ_CAP_BUTT, AZ_JOIN_MITER_OR_BEVEL, AZ_FILTER_TYPE_COLOR_MATRIX};
use azure::{AZ_FILTER_TYPE_FLOOD, AZ_FILTER_TYPE_GAUSSIAN_BLUR, AZ_FILTER_TYPE_LINEAR_TRANSFER};
use azure::{AZ_FILTER_TYPE_TABLE_TRANSFER, AZ_IN_COLOR_MATRIX_IN, AZ_IN_COMPOSITE_IN};
use azure::{AZ_IN_FLOOD_IN, AZ_IN_GAUSSIAN_BLUR_IN, AZ_IN_LINEAR_TRANSFER_IN};
use azure::{AZ_ATT_COLOR_MATRIX_MATRIX, AZ_ATT_FLOOD_COLOR, AZ_ATT_GAUSSIAN_BLUR_STD_DEVIATION};
use azure::{AZ_FILTER_TYPE_COMPOSITE, AZ_IN_TABLE_TRANSFER_IN, AZ_ATT_LINEAR_TRANSFER_SLOPE_R};
use azure::{AZ_ATT_LINEAR_TRANSFER_SLOPE_G, AZ_ATT_LINEAR_TRANSFER_SLOPE_B};
use azure::{AZ_ATT_LINEAR_TRANSFER_SLOPE_A, AZ_ATT_LINEAR_TRANSFER_INTERCEPT_R};
use azure::{AZ_ATT_LINEAR_TRANSFER_INTERCEPT_G, AZ_ATT_LINEAR_TRANSFER_INTERCEPT_B};
use azure::{AZ_ATT_LINEAR_TRANSFER_INTERCEPT_A, AZ_ATT_TABLE_TRANSFER_TABLE_R};
use azure::{AZ_ATT_TABLE_TRANSFER_TABLE_G, AZ_ATT_TABLE_TRANSFER_TABLE_B};
use azure::{AZ_ATT_TABLE_TRANSFER_TABLE_A, AZ_ATT_TRANSFER_DISABLE_R};
use azure::{AZ_ATT_TRANSFER_DISABLE_G, AZ_ATT_TRANSFER_DISABLE_B};
use azure::{AZ_ATT_TRANSFER_DISABLE_A};
use azure::{AzPoint, AzRect, AzFloat, AzIntSize, AzColor, AzColorPatternRef, AzGradientStopsRef};
use azure::{AzStrokeOptions, AzDrawOptions, AzSurfaceFormat, AzFilter, AzDrawSurfaceOptions};
use azure::{AzBackendType, AzDrawTargetRef, AzSourceSurfaceRef, AzDataSourceSurfaceRef};
use azure::{AzScaledFontRef, AzGlyphRenderingOptionsRef, AzExtendMode, AzGradientStop};
use azure::{AzCompositionOp, AzJoinStyle, AzCapStyle};
use azure::{struct__AzColor, struct__AzGlyphBuffer};
use azure::{struct__AzDrawOptions, struct__AzDrawSurfaceOptions, struct__AzIntSize};
use azure::{struct__AzPoint, struct__AzRect, struct__AzStrokeOptions, struct__AzMatrix5x4};
use azure::{AzCreateColorPattern, AzCreateDrawTarget, AzCreateDrawTargetForData};
use azure::{AzDataSourceSurfaceGetData, AzDataSourceSurfaceGetStride};
use azure::{AzDrawTargetClearRect};
use azure::{AzDrawTargetCreateSourceSurfaceFromData};
use azure::{AzDrawTargetDrawSurface, AzDrawTargetFillRect, AzDrawTargetFlush};
use azure::{AzDrawTargetGetSize, AzDrawTargetGetSnapshot, AzDrawTargetSetTransform};
use azure::{AzDrawTargetStrokeLine, AzDrawTargetStrokeRect, AzDrawTargetFillGlyphs};
use azure::{AzDrawTargetCreateGradientStops, AzDrawTargetGetFormat};
use azure::{AzReleaseDrawTarget, AzReleasePattern, AzReleaseGradientStops};
use azure::{AzReleaseSourceSurface, AzRetainDrawTarget};
use azure::{AzSourceSurfaceGetDataSurface, AzSourceSurfaceGetFormat};
use azure::{AzSourceSurfaceGetSize, AzCreateDrawTargetSkiaWithGrContextAndFBO};
use azure::{AzCreatePathBuilder, AzPathBuilderRef, AzPathBuilderMoveTo, AzPathBuilderLineTo};
use azure::{AzDrawTargetStroke, AzPathBuilderArc, AzPathBuilderFinish, AzReleasePathBuilder};
use azure::{AzDrawTargetFill, AzPathRef, AzReleasePath, AzDrawTargetPushClip, AzDrawTargetPopClip};
use azure::{AzLinearGradientPatternRef, AzRadialGradientPatternRef, AzSurfacePatternRef, AzMatrix, AzPatternRef};
use azure::{AzCreateLinearGradientPattern, AzCreateRadialGradientPattern, AzCreateSurfacePattern, AzDrawTargetPushClipRect};
use azure::{AzCloneLinearGradientPattern, AzCloneRadialGradientPattern, AzCloneSurfacePattern};
use azure::{AzDrawTargetDrawSurfaceWithShadow, AzDrawTargetCreateShadowDrawTarget};
use azure::{AzDrawTargetCreateSimilarDrawTarget, AzDrawTargetGetTransform};
use azure::{AzFilterNodeSetSourceSurfaceInput, AzReleaseFilterNode, AzDrawTargetCreateFilter};
use azure::{AzFilterNodeSetColorAttribute, AzFilterNodeSetFloatAttribute};
use azure::{AzFilterNodeSetMatrix5x4Attribute, AzFilterNodeSetFilterNodeInput};
use azure::{AzFilterNodeSetFloatArrayAttribute, AzFilterNodeSetBoolAttribute};
use azure::{AzDrawTargetDrawFilter, AzFilterNodeRef, AzFilterType};
use azure::{AzPathBuilderBezierTo, AzPathBuilderQuadraticBezierTo};
use azure::{AzPathBuilderCurrentPoint, AzPathBuilderClose};

use geom::matrix2d::Matrix2D;
use geom::point::Point2D;
use geom::rect::Rect;
use geom::size::Size2D;
use libc::types::common::c99::{uint8_t, uint16_t};
use libc::size_t;
use skia::{SkiaGrGLNativeContextRef, SkiaGrContextRef};
use skia::{SkiaSkNativeSharedGLContextRef, SkiaSkNativeSharedGLContextCreate};
use skia::{SkiaSkNativeSharedGLContextGetFBOID, SkiaSkNativeSharedGLContextGetGrContext};
use skia::{SkiaSkNativeSharedGLContextRelease, SkiaSkNativeSharedGLContextRetain};
use skia::{SkiaSkNativeSharedGLContextMakeCurrent, SkiaSkNativeSharedGLContextStealSurface};
use skia::{SkiaSkNativeSharedGLContextFlush, SkiaGrGLSharedSurfaceRef};
use std::mem;
use std::ptr;
use std::slice;
use std::sync::Arc;

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

// FIXME: Should have a class hierarchy here starting with Pattern.
pub struct ColorPattern {
    color: Color,
    pub azure_color_pattern: AzColorPatternRef,
}

impl Drop for ColorPattern {
    fn drop(&mut self) {
        unsafe {
            AzReleasePattern(self.azure_color_pattern);
        }
    }
}

impl Clone for ColorPattern {
    fn clone(&self) -> ColorPattern {
        ColorPattern::new(self.color.clone())
    }
}

impl ColorPattern {
    pub fn new(color: Color) -> ColorPattern {
        unsafe {
            ColorPattern {
                color: color,
                azure_color_pattern: AzCreateColorPattern(&mut color.clone())
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

#[repr(u8)]
#[derive(Clone, Copy, PartialEq)]
pub enum JoinStyle {
    Bevel = 0,
    Round = 1,
    Miter = 2,
    MiterOrBevel = 3,
}

impl JoinStyle {
    fn as_azure_join_style(self) -> AzJoinStyle {
        self as AzJoinStyle
    }
}

#[repr(u8)]
#[derive(Clone, Copy, PartialEq)]
pub enum CapStyle {
    Butt = 0,
    Round = 1,
    Square = 2,
}

impl CapStyle {
    fn as_azure_cap_style(self) -> AzCapStyle {
        self as AzCapStyle
    }
}

#[allow(non_snake_case)]
#[derive(Clone)]
pub struct StrokeOptions<'a> {
    pub line_width: AzFloat,
    pub miter_limit: AzFloat,
    pub mDashPattern: &'a[AzFloat],
    pub line_join: JoinStyle,
    pub line_cap: CapStyle,
}

impl<'a> StrokeOptions<'a> {
    pub fn new(line_width: AzFloat, line_join: JoinStyle, line_cap: CapStyle, miter_limit: AzFloat,
               dash_pattern: &'a[AzFloat]) -> StrokeOptions {
        StrokeOptions {
            line_width: line_width,
            miter_limit: miter_limit,
            mDashPattern: dash_pattern,
            line_join: line_join,
            line_cap: line_cap,
        }
    }

    fn as_azure_stroke_options(&self) -> AzStrokeOptions {
        struct__AzStrokeOptions {
            mLineWidth: self.line_width,
            mMiterLimit: self.miter_limit,
            mDashPattern: self.mDashPattern.as_ptr(),
            mDashLength: self.mDashPattern.len() as size_t,
            mDashOffset: 0.0 as AzFloat,
            mLineJoin: self.line_join.as_azure_join_style(),
            mLineCap: self.line_cap.as_azure_cap_style(),
        }
    }
}

#[derive(Clone)]
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

#[derive(Copy, Clone)]
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


#[derive(Clone, PartialEq)]
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
    pub backing: DrawTargetBacking,
}

impl Drop for DrawTarget {
    fn drop(&mut self) {
        unsafe {
            AzReleaseDrawTarget(self.azure_draw_target);
        }
    }
}

impl PartialEq for DrawTarget {
    #[inline]
    fn eq(&self, other: &DrawTarget) -> bool {
        self.azure_draw_target == other.azure_draw_target
    }
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
            backing: DrawTargetBacking::Empty,
        }
    }

    pub fn new_with_data(backend: BackendType,
                         mut data: Vec<u8>,
                         offset: usize,
                         size: Size2D<i32>,
                         stride: i32,
                         format: SurfaceFormat) -> DrawTarget {
        assert!((data.len() - offset) as i32 >= stride * size.height);
        let azure_draw_target = unsafe {
            AzCreateDrawTargetForData(backend.as_azure_backend_type(),
                                      data.as_mut_ptr().offset(offset as isize),
                                      &mut size.as_azure_int_size(),
                                      stride,
                                      format.as_azure_surface_format())
        };
        if azure_draw_target.is_null() {
            panic!("null azure draw target");
        }
        DrawTarget {
            azure_draw_target: azure_draw_target,
            backing: DrawTargetBacking::Data(Arc::new(data)),
        }
    }

    pub fn new_with_fbo(backend: BackendType,
                        native_graphics_context: SkiaGrGLNativeContextRef,
                        size: Size2D<i32>,
                        format: SurfaceFormat) -> DrawTarget {
        assert!(backend == BackendType::Skia);
        let native_surface = native_graphics_context as SkiaGrGLSharedSurfaceRef;
        let skia_context = unsafe {
            SkiaSkNativeSharedGLContextCreate(native_graphics_context, size.width, size.height)
        };

        if skia_context.is_null() {
            panic!("null skia shared gl context");
        }

        let gr_context = unsafe {
            SkiaSkNativeSharedGLContextGetGrContext(skia_context)
        };

        let fbo_id = unsafe {
            SkiaSkNativeSharedGLContextGetFBOID(skia_context)
        };

        let azure_draw_target = unsafe {
            AzCreateDrawTargetSkiaWithGrContextAndFBO(gr_context,
                                                      fbo_id,
                                                      &mut size.as_azure_int_size(),
                                                      format.as_azure_surface_format())
        };
        if azure_draw_target.is_null() {
            panic!("null azure draw target");
        }
        DrawTarget {
            azure_draw_target: azure_draw_target,
            backing: DrawTargetBacking::SkiaContext(skia_context)
        }
    }

    pub fn clone(&self) -> DrawTarget {
        unsafe {
            AzRetainDrawTarget(self.azure_draw_target);
        }
        DrawTarget {
            azure_draw_target: self.azure_draw_target,
            backing: self.backing.clone(),
        }
    }

    pub fn make_current(&self) {
        self.backing.make_current();
    }

    /// Consumes this draw target and returns the underlying draw target backing.
    pub fn steal_draw_target_backing(mut self) -> DrawTargetBacking {
        let mut backing = DrawTargetBacking::Empty;
        mem::swap(&mut backing, &mut self.backing);
        backing
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
        }
        self.backing.flush();
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

    pub fn stroke(&self,
                  path: &Path,
                  pattern: &ColorPattern,
                  stroke_options: &StrokeOptions,
                  draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetStroke(self.azure_draw_target,
                               path.azure_path,
                               pattern.azure_color_pattern,
                               &stroke_options.as_azure_stroke_options(),
                               &draw_options.as_azure_draw_options());
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
                       filter: &FilterNode,
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
                data.as_ptr(),
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
                backing: DrawTargetBacking::Empty,
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
                backing: DrawTargetBacking::Empty,
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
                    mem::transmute::<_,*const AzGradientStop>(gradient_stops.as_ptr()),
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

pub enum DrawTargetBacking {
    Empty, // The backing is completely owned by the DrawTarget.
    Data(Arc<Vec<u8>>),
    SkiaContext(SkiaSkNativeSharedGLContextRef),
}

impl Drop for DrawTargetBacking {
    fn drop(&mut self) {
        match *self {
            DrawTargetBacking::Empty | DrawTargetBacking::Data(_) => { }
            DrawTargetBacking::SkiaContext(skia_context) => unsafe {
                SkiaSkNativeSharedGLContextRelease(skia_context);
            }
        }
    }
}

impl DrawTargetBacking {
    pub fn clone(&self) -> DrawTargetBacking {
        match *self {
            DrawTargetBacking::Empty => DrawTargetBacking::Empty,
            DrawTargetBacking::Data(ref arc_data) => DrawTargetBacking::Data(arc_data.clone()),
            DrawTargetBacking::SkiaContext(skia_context) => {
                unsafe { SkiaSkNativeSharedGLContextRetain(skia_context); }
                DrawTargetBacking::SkiaContext(skia_context)
            }
        }
    }

    pub fn make_current(&self) {
        match *self {
            DrawTargetBacking::Empty | DrawTargetBacking::Data(_) => { }
            DrawTargetBacking::SkiaContext(skia_context) => unsafe {
                SkiaSkNativeSharedGLContextMakeCurrent(skia_context);
            }
        }
    }

    pub fn flush(&self) {
        match *self {
            DrawTargetBacking::Empty | DrawTargetBacking::Data(_) => { },
            DrawTargetBacking::SkiaContext(skia_context) => unsafe {
                SkiaSkNativeSharedGLContextFlush(skia_context);
            }
        }
    }
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
#[derive(Clone)]
pub struct GradientStop {
    pub offset: AzFloat,
    pub color: Color,
}

#[repr(i32)]
#[derive(Clone, PartialEq)]
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
    pub fn with_data<F: FnOnce(&[u8])>(&self, f: F) {
        unsafe {
            let buf = AzDataSourceSurfaceGetData(self.azure_data_source_surface) as *const u8;
            let len = self.stride() * self.size().height;
            let slice = slice::from_raw_parts(buf, len as usize);
            f(slice)
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

    /// Adds a quadratic Bézier curve to the current figure.
    pub fn quadratic_curve_to(&self,
                              control_point: &Point2D<AzFloat>,
                              end_point: &Point2D<AzFloat>) {
        unsafe {
            AzPathBuilderQuadraticBezierTo(self.azure_path_builder,
                                           &control_point.as_azure_point(),
                                           &end_point.as_azure_point())
        }
    }

    /// Adds a cubic Bézier curve to the current figure.
    pub fn bezier_curve_to(&self,
                           control_point_1: &Point2D<AzFloat>,
                           control_point_2: &Point2D<AzFloat>,
                           control_point_3: &Point2D<AzFloat>) {
        unsafe {
            AzPathBuilderBezierTo(self.azure_path_builder,
                                  &control_point_1.as_azure_point(),
                                  &control_point_2.as_azure_point(),
                                  &control_point_3.as_azure_point())
        }
    }

    /// Returns the current last point of the current path.
    pub fn get_current_point(&self) -> AzPoint {
        unsafe {
            AzPathBuilderCurrentPoint(self.azure_path_builder)
        }
    }

    /// Closes the current path.
    pub fn close(&self) {
        unsafe {
            AzPathBuilderClose(self.azure_path_builder)
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

impl Clone for LinearGradientPattern {
    fn clone(&self) -> LinearGradientPattern {
        unsafe {
            LinearGradientPattern {
                azure_linear_gradient_pattern:
                    AzCloneLinearGradientPattern(self.azure_linear_gradient_pattern),
            }
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

pub struct RadialGradientPattern {
    pub azure_radial_gradient_pattern: AzRadialGradientPatternRef,
}

impl Drop for RadialGradientPattern {
    fn drop(&mut self) {
        unsafe {
            AzReleasePattern(self.azure_radial_gradient_pattern);
        }
    }
}

impl Clone for RadialGradientPattern {
    fn clone(&self) -> RadialGradientPattern {
        unsafe {
            RadialGradientPattern {
                azure_radial_gradient_pattern:
                    AzCloneRadialGradientPattern(self.azure_radial_gradient_pattern),
            }
        }
    }
}

impl RadialGradientPattern {
    pub fn new(center1: &Point2D<AzFloat>,
               center2: &Point2D<AzFloat>,
               radius1: AzFloat,
               radius2: AzFloat,
               stops: GradientStops,
               matrix: &Matrix2D<AzFloat>)
               -> RadialGradientPattern {
        unsafe {
            RadialGradientPattern {
                azure_radial_gradient_pattern:
                    AzCreateRadialGradientPattern(mem::transmute::<_,*const AzPoint>(center1),
                                                  mem::transmute::<_,*const AzPoint>(center2),
                                                  radius1,
                                                  radius2,
                                                  stops.azure_gradient_stops,
                                                  mem::transmute::<_,*const AzMatrix>(matrix)),
            }
        }
    }
}

pub struct SurfacePattern {
    pub azure_surface_pattern: AzSurfacePatternRef,
}

impl Drop for SurfacePattern {
    fn drop(&mut self) {
        unsafe {
            AzReleasePattern(self.azure_surface_pattern);
        }
    }
}

impl Clone for SurfacePattern {
    fn clone(&self) -> SurfacePattern {
        unsafe {
            SurfacePattern {
                azure_surface_pattern:
                    AzCloneSurfacePattern(self.azure_surface_pattern),
            }
        }
    }
}

impl SurfacePattern {
    pub fn new(surface: AzSourceSurfaceRef)
               -> SurfacePattern {
        unsafe {
            SurfacePattern {
                azure_surface_pattern:
                    AzCreateSurfacePattern(surface),
            }
        }
    }
}

pub enum PatternRef<'a> {
    Color(&'a ColorPattern),
    LinearGradient(&'a LinearGradientPattern),
    RadialGradient(&'a RadialGradientPattern),
    Surface(&'a SurfacePattern),
}

impl<'a> PatternRef<'a> {
    fn as_azure_pattern(&self) -> AzPatternRef {
        match *self {
            PatternRef::Color(color_pattern) => {
                color_pattern.azure_color_pattern
            },
            PatternRef::LinearGradient(linear_gradient_pattern) => {
                linear_gradient_pattern.azure_linear_gradient_pattern
            },
            PatternRef::RadialGradient(radial_gradient_pattern) => {
                radial_gradient_pattern.azure_radial_gradient_pattern
            },
            PatternRef::Surface(surface_pattern) => {
                surface_pattern.azure_surface_pattern
            },
        }
    }
}

#[derive(Clone)]
pub enum Pattern {
    Color(ColorPattern),
    LinearGradient(LinearGradientPattern),
    RadialGradient(RadialGradientPattern),
    Surface(SurfacePattern),
}

impl Pattern {
    pub fn to_pattern_ref(&self) -> PatternRef {
        match *self {
            Pattern::Color(ref color_pattern) => PatternRef::Color(color_pattern),
            Pattern::LinearGradient(ref linear_gradient_pattern) => {
                PatternRef::LinearGradient(linear_gradient_pattern)
            },
            Pattern::RadialGradient(ref radial_gradient_pattern) => {
                PatternRef::RadialGradient(radial_gradient_pattern)
            },
            Pattern::Surface(ref surface_pattern) => {
                PatternRef::Surface(surface_pattern)
            },
        }
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

pub struct ColorMatrixInput;

pub struct CompositeInput;

pub struct FloodFilterInput;

pub struct GaussianBlurInput;

pub struct LinearTransferInput;

pub struct TableTransferInput;

pub trait FilterInputIndex {
    fn azure_index(&self) -> u32;
}

impl FilterInputIndex for ColorMatrixInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_COLOR_MATRIX_IN
    }
}

impl FilterInputIndex for CompositeInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_COMPOSITE_IN
    }
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

impl FilterInputIndex for LinearTransferInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_LINEAR_TRANSFER_IN
    }
}

impl FilterInputIndex for TableTransferInput {
    fn azure_index(&self) -> u32 {
        AZ_IN_TABLE_TRANSFER_IN
    }
}

pub trait FilterAttribute {
    fn set(&self, filter_node: &FilterNode);
}

pub enum ColorMatrixAttribute {
    Matrix(Matrix5x4),
}

pub enum FloodAttribute {
    Color(Color),
}

pub enum GaussianBlurAttribute {
    StdDeviation(AzFloat),
}

pub enum LinearTransferAttribute {
    DisableR(bool),
    DisableG(bool),
    DisableB(bool),
    DisableA(bool),
    SlopeR(AzFloat),
    SlopeG(AzFloat),
    SlopeB(AzFloat),
    SlopeA(AzFloat),
    InterceptR(AzFloat),
    InterceptG(AzFloat),
    InterceptB(AzFloat),
    InterceptA(AzFloat),
}

pub enum TableTransferAttribute<'a> {
    DisableR(bool),
    DisableG(bool),
    DisableB(bool),
    DisableA(bool),
    TableR(&'a [AzFloat]),
    TableG(&'a [AzFloat]),
    TableB(&'a [AzFloat]),
    TableA(&'a [AzFloat]),
}

impl FilterAttribute for ColorMatrixAttribute {
    fn set(&self, filter_node: &FilterNode) {
        let ColorMatrixAttribute::Matrix(ref value) = *self;
        unsafe {
            AzFilterNodeSetMatrix5x4Attribute(filter_node.azure_filter_node,
                                              AZ_ATT_COLOR_MATRIX_MATRIX,
                                              &value.as_azure_matrix_5x4())
        }
    }
}

impl FilterAttribute for FloodAttribute {
    fn set(&self, filter_node: &FilterNode) {
        let FloodAttribute::Color(value) = *self;
        unsafe {
            AzFilterNodeSetColorAttribute(filter_node.azure_filter_node,
                                          AZ_ATT_FLOOD_COLOR,
                                          &value)
        }
    }
}

impl FilterAttribute for GaussianBlurAttribute {
    fn set(&self, filter_node: &FilterNode) {
        let GaussianBlurAttribute::StdDeviation(value) = *self;
        unsafe {
            AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                          AZ_ATT_GAUSSIAN_BLUR_STD_DEVIATION,
                                          value)
        }
    }
}

impl FilterAttribute for LinearTransferAttribute {
    fn set(&self, filter_node: &FilterNode) {
        unsafe {
            match *self {
                LinearTransferAttribute::DisableR(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_R,
                                                 value)
                }
                LinearTransferAttribute::DisableG(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_G,
                                                 value)
                }
                LinearTransferAttribute::DisableB(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_B,
                                                 value)
                }
                LinearTransferAttribute::DisableA(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_A,
                                                 value)
                }
                LinearTransferAttribute::SlopeR(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_SLOPE_R,
                                                  value)
                }
                LinearTransferAttribute::SlopeG(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_SLOPE_G,
                                                  value)
                }
                LinearTransferAttribute::SlopeB(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_SLOPE_B,
                                                  value)
                }
                LinearTransferAttribute::SlopeA(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_SLOPE_A,
                                                  value)
                }
                LinearTransferAttribute::InterceptR(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_INTERCEPT_R,
                                                  value)
                }
                LinearTransferAttribute::InterceptG(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_INTERCEPT_G,
                                                  value)
                }
                LinearTransferAttribute::InterceptB(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_INTERCEPT_B,
                                                  value)
                }
                LinearTransferAttribute::InterceptA(value) => {
                    AzFilterNodeSetFloatAttribute(filter_node.azure_filter_node,
                                                  AZ_ATT_LINEAR_TRANSFER_INTERCEPT_A,
                                                  value)
                }
            }
        }
    }
}

impl<'a> FilterAttribute for TableTransferAttribute<'a> {
    fn set(&self, filter_node: &FilterNode) {
        unsafe {
            match *self {
                TableTransferAttribute::DisableR(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_R,
                                                 value)
                }
                TableTransferAttribute::DisableG(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_G,
                                                 value)
                }
                TableTransferAttribute::DisableB(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_B,
                                                 value)
                }
                TableTransferAttribute::DisableA(value) => {
                    AzFilterNodeSetBoolAttribute(filter_node.azure_filter_node,
                                                 AZ_ATT_TRANSFER_DISABLE_A,
                                                 value)
                }
                TableTransferAttribute::TableR(value) => {
                    AzFilterNodeSetFloatArrayAttribute(filter_node.azure_filter_node,
                                                       AZ_ATT_TABLE_TRANSFER_TABLE_R,
                                                       value.as_ptr(),
                                                       value.len() as u32)
                }
                TableTransferAttribute::TableG(value) => {
                    AzFilterNodeSetFloatArrayAttribute(filter_node.azure_filter_node,
                                                       AZ_ATT_TABLE_TRANSFER_TABLE_G,
                                                       value.as_ptr(),
                                                       value.len() as u32)
                }
                TableTransferAttribute::TableB(value) => {
                    AzFilterNodeSetFloatArrayAttribute(filter_node.azure_filter_node,
                                                       AZ_ATT_TABLE_TRANSFER_TABLE_B,
                                                       value.as_ptr(),
                                                       value.len() as u32)
                }
                TableTransferAttribute::TableA(value) => {
                    AzFilterNodeSetFloatArrayAttribute(filter_node.azure_filter_node,
                                                       AZ_ATT_TABLE_TRANSFER_TABLE_A,
                                                       value.as_ptr(),
                                                       value.len() as u32)
                }
            }
        }
    }
}

pub enum FilterType {
    ColorMatrix,
    Composite,
    Flood,
    GaussianBlur,
    LinearTransfer,
    TableTransfer,
}

impl FilterType {
    pub fn as_azure_filter_type(self) -> AzFilterType {
        match self {
            FilterType::ColorMatrix => AZ_FILTER_TYPE_COLOR_MATRIX,
            FilterType::Composite => AZ_FILTER_TYPE_COMPOSITE,
            FilterType::Flood => AZ_FILTER_TYPE_FLOOD,
            FilterType::GaussianBlur => AZ_FILTER_TYPE_GAUSSIAN_BLUR,
            FilterType::LinearTransfer => AZ_FILTER_TYPE_LINEAR_TRANSFER,
            FilterType::TableTransfer => AZ_FILTER_TYPE_TABLE_TRANSFER,
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

#[derive(PartialEq, Clone, Debug)]
pub struct Matrix5x4 {
    pub m11: AzFloat, pub m12: AzFloat, pub m13: AzFloat, pub m14: AzFloat,
    pub m21: AzFloat, pub m22: AzFloat, pub m23: AzFloat, pub m24: AzFloat,
    pub m31: AzFloat, pub m32: AzFloat, pub m33: AzFloat, pub m34: AzFloat,
    pub m41: AzFloat, pub m42: AzFloat, pub m43: AzFloat, pub m44: AzFloat,
    pub m51: AzFloat, pub m52: AzFloat, pub m53: AzFloat, pub m54: AzFloat,
}

impl Matrix5x4 {
    #[inline]
    pub fn as_azure_matrix_5x4(&self) -> struct__AzMatrix5x4 {
        struct__AzMatrix5x4 {
            _11: self.m11, _12: self.m12, _13: self.m13, _14: self.m14,
            _21: self.m21, _22: self.m22, _23: self.m23, _24: self.m24,
            _31: self.m31, _32: self.m32, _33: self.m33, _34: self.m34,
            _41: self.m41, _42: self.m42, _43: self.m43, _44: self.m44,
            _51: self.m51, _52: self.m52, _53: self.m53, _54: self.m54,
        }
    }
}


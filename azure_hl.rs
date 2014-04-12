// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

//! High-level bindings to Azure.

use azure::{AZ_CAP_BUTT, AZ_JOIN_MITER_OR_BEVEL};
use azure::{AzPoint, AzRect, AzFloat, AzIntSize, AzColor, AzColorPatternRef};
use azure::{AzStrokeOptions, AzDrawOptions, AzSurfaceFormat, AzFilter, AzDrawSurfaceOptions};
use azure::{AzBackendType, AzDrawTargetRef, AzSourceSurfaceRef, AzDataSourceSurfaceRef};
use azure::{AzScaledFontRef, AzGlyphRenderingOptionsRef};
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
use azure::{AzReleaseColorPattern, AzReleaseDrawTarget};
use azure::{AzReleaseSourceSurface, AzRetainDrawTarget};
use azure::{AzSourceSurfaceGetDataSurface, AzSourceSurfaceGetFormat};
use azure::{AzSourceSurfaceGetSize, AzCreateSkiaDrawTargetForFBO, AzSkiaGetCurrentGLContext};
use azure::{AzSkiaSharedGLContextMakeCurrent, AzSkiaSharedGLContextStealSurface};
use azure::{AzSkiaSharedGLContextFlush, AzSkiaGrGLSharedSurfaceRef};
use azure::{AzCreatePathBuilder, AzPathBuilderRef, AzPathBuilderMoveTo, AzPathBuilderLineTo, AzPathBuilderFinish, AzReleasePathBuilder};
use azure::{AzDrawTargetFill, AzPathRef, AzReleasePath, AzDrawTargetPushClip, AzDrawTargetPopClip};

use sync::Arc;
use geom::matrix2d::Matrix2D;
use geom::point::Point2D;
use geom::rect::Rect;
use geom::size::Size2D;
use layers::platform::surface::{NativeGraphicsMetadata, NativePaintingGraphicsContext};
use libc::types::common::c99::{uint8_t, uint16_t};
use libc::size_t;
use std::cast;
use std::ptr;
use std::ptr::null;
use std::slice;

#[cfg(target_os="linux")]
use libc::c_void;

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
    r: AzFloat,
    g: AzFloat,
    b: AzFloat,
    a: AzFloat,
}

impl Color {
    fn as_azure_color(&self) -> AzColor {
        struct__AzColor { r: self.r, g: self.g, b: self.b, a: self.a }
    }
}

pub fn Color(r: AzFloat, g: AzFloat, b: AzFloat, a: AzFloat) -> Color {
    Color { r: r, g: g, b: b, a: a }
}


// FIXME: Should have a class hierarchy here starting with Pattern.
pub struct ColorPattern {
    azure_color_pattern: AzColorPatternRef,
}

impl Drop for ColorPattern {
    fn drop(&mut self) {
        unsafe {
            AzReleaseColorPattern(self.azure_color_pattern);
        }
    }
}

pub fn ColorPattern(color: Color) -> ColorPattern {
    unsafe {
        ColorPattern {
            azure_color_pattern: AzCreateColorPattern(&color.as_azure_color())
        }
    }
}

pub enum CompositionOp {
    OverOp,
    AddOp,
    AtopOp,
    OutOp,
    InOp,
    SourceOp,
    DestInOp,
    DestOutOp,
    DestOverOp,
    DestAtopOp,
    XorOp,
    MultiplyOp,
    ScreenOp,
    OverlayOp,
    DarkenOp,
    LightenOp,
    ColorDodgeOp,
    ColorBurnOp,
    HardLightOp,
    SoftLightOp,
    DifferenceOp,
    ExclusionOp,
    HueOp,
    SaturationOp,
    ColorOp,
    LuminosityOp,
}

pub struct StrokeOptions {
    line_width: AzFloat,
    miter_limit: AzFloat,
    mDashPattern: *AzFloat,
    mDashLength: size_t,
    fields: uint8_t
}

impl StrokeOptions {
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

pub fn StrokeOptions(line_width: AzFloat,
                  miter_limit: AzFloat) -> StrokeOptions {
    StrokeOptions {
        line_width: line_width,
        miter_limit: miter_limit,
        mDashPattern: null(),
        mDashLength: 0,
        fields: AZ_CAP_BUTT as u8 << 4 | AZ_JOIN_MITER_OR_BEVEL as u8
    }
}

pub struct DrawOptions {
    alpha: AzFloat,
    fields: uint16_t,
}

impl DrawOptions {
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

    pub fn set_snapping(&mut self, style: u8) {
        self.fields = self.fields & 0b1111_0111_1111_1111_u16;
        let style = ((style & 1) as u16) << 11;
        self.fields = self.fields | style;
    }
}


pub fn DrawOptions(alpha: AzFloat, fields: uint16_t) -> DrawOptions {
    DrawOptions {
        alpha : alpha,
        fields : fields,
    }
}

pub enum SurfaceFormat {
    B8G8R8A8,
    B8G8R8X8,
    R5G6B5,
    A8
}

impl SurfaceFormat {
    fn as_azure_surface_format(self) -> AzSurfaceFormat {
        self as AzSurfaceFormat
    }

    pub fn new(azure_surface_format: AzSurfaceFormat) -> SurfaceFormat {
        match azure_surface_format {
            0 => B8G8R8A8,
            1 => B8G8R8X8,
            2 => R5G6B5,
            3 => A8,
            _ => fail!(~"SurfaceFormat::new(): unknown Azure surface format")
        }
    }
}

pub enum Filter {
    Linear,
    Point
}

impl Filter {
    pub fn as_azure_filter(self) -> AzFilter {
        self as AzFilter
    }
}

pub struct DrawSurfaceOptions {
    filter: Filter,
    sampling_bounds: bool,
}

impl DrawSurfaceOptions {
    fn as_azure_draw_surface_options(&self) -> AzDrawSurfaceOptions {
        struct__AzDrawSurfaceOptions {
            fields: ((self.filter as int) | (if self.sampling_bounds { 8 } else { 0 })) as u32
        }
    }
}


pub fn DrawSurfaceOptions(filter: Filter, sampling_bounds: bool) -> DrawSurfaceOptions {
    DrawSurfaceOptions {
        filter: filter,
        sampling_bounds: sampling_bounds,
    }
}


#[deriving(Clone, Eq)]
pub enum BackendType {
    NoBackend,
    Direct2DBackend,
    CoreGraphicsBackend,
    CoreGraphicsAcceleratedBackend,
    CairoBackend,
    SkiaBackend,
    RecordingBackend
}

impl BackendType {
    pub fn as_azure_backend_type(self) -> AzBackendType {
        match self {
            NoBackend                      => 0,
            Direct2DBackend                => 1,
            CoreGraphicsBackend            => 2,
            CoreGraphicsAcceleratedBackend => 3,
            CairoBackend                   => 4,
            SkiaBackend                    => 5,
            RecordingBackend               => 6,
        }
    }
}

pub struct DrawTarget {
    azure_draw_target: AzDrawTargetRef,
    data: Option<Arc<~[u8]>>,
    skia_context: Option<AzSkiaSharedGLContextRef>
}

impl Drop for DrawTarget {
    fn drop(&mut self) {
        unsafe {
            match self.skia_context {
                None => {}
                Some(ctx_ref) => { AzReleaseSkiaSharedGLContext(ctx_ref); }
            }
            AzReleaseDrawTarget(self.azure_draw_target);
        }
    }
}

/// Contains the GL resources that Skia was holding onto that may be safely extracted. At the
/// moment this consists simply of the native surface.
pub struct StolenGLResources {
    surface: AzSkiaGrGLSharedSurfaceRef,
}

impl DrawTarget {
    pub fn new(backend: BackendType, size: Size2D<i32>, format: SurfaceFormat)
                   -> DrawTarget {
        unsafe {
            let azure_draw_target = AzCreateDrawTarget(backend.as_azure_backend_type(),
                                                       &size.as_azure_int_size(),
                                                       format.as_azure_surface_format());
            if azure_draw_target == ptr::null() { fail!(~"null azure draw target"); }
            DrawTarget {
                azure_draw_target: azure_draw_target,
                data: None,
                skia_context: None
            }
        }
    }

    pub fn new_with_data(backend: BackendType,
                         data: ~[u8],
                         offset: uint,
                         size: Size2D<i32>,
                         stride: i32,
                         format: SurfaceFormat) -> DrawTarget {
        unsafe {
            assert!((data.len() - offset) as i32 >= stride * size.height);
            let azure_draw_target =
                AzCreateDrawTargetForData(backend.as_azure_backend_type(),
                                          &data[offset],
                                          &size.as_azure_int_size(),
                                          stride,
                                          format.as_azure_surface_format());
            if azure_draw_target == ptr::null() { fail!(~"null azure draw target"); }
            DrawTarget {
                azure_draw_target: azure_draw_target,
                data: Some(Arc::new(data)),
                skia_context: None
            }
        }
    }

    pub fn new_with_fbo(backend: BackendType,
                        native_graphics_context: &NativePaintingGraphicsContext,
                        size: Size2D<i32>,
                        format: SurfaceFormat) -> DrawTarget {
        assert!(backend == SkiaBackend);
        unsafe {
            let native_graphics_context = cast::transmute(native_graphics_context);
            let skia_context = AzCreateSkiaSharedGLContext(native_graphics_context,
                                                           &size.as_azure_int_size());
            let azure_draw_target = AzCreateSkiaDrawTargetForFBO(skia_context,
                                                                 &size.as_azure_int_size(),
                                                                 format.as_azure_surface_format());
            if azure_draw_target == ptr::null() {
                fail!(~"null azure draw target");
            }
            DrawTarget {
                azure_draw_target: azure_draw_target,
                data: None,
                skia_context: Some(skia_context)
            }
        }
    }

    pub fn clone(&self) -> DrawTarget {
        unsafe {
            AzRetainDrawTarget(self.azure_draw_target);
            if self.skia_context.is_some() {
                AzRetainSkiaSharedGLContext(self.skia_context.unwrap());
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
        match self.skia_context {
            None => None,
            Some(ctx) => {
                unsafe {
                    Some(StolenGLResources {
                        surface: AzSkiaSharedGLContextStealSurface(ctx),
                    })
                }
            }
        }
    }

    pub fn get_size(&self) -> AzIntSize {
        unsafe {
            AzDrawTargetGetSize(self.azure_draw_target)
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
            AzDrawTargetClearRect(self.azure_draw_target, &rect.as_azure_rect());
        }
    }

    pub fn fill(&self, path: &Path, pattern: &ColorPattern, draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetFill(self.azure_draw_target,
                             path.azure_path,
                             pattern.azure_color_pattern,
                             &draw_options.as_azure_draw_options());
        }
    }

    pub fn fill_rect(&self,
                     rect: &Rect<AzFloat>,
                     pattern: &ColorPattern,
                     draw_options: Option<&DrawOptions>) {
        unsafe {
            let draw_options = draw_options.map(|draw_options| {
                draw_options.as_azure_draw_options()
            });
            let draw_options = match draw_options {
                None => ptr::null(),
                Some(draw_options) => {
                    let draw_options: *AzDrawOptions = &draw_options;
                    draw_options
                }
            };
            AzDrawTargetFillRect(self.azure_draw_target,
                                 &rect.as_azure_rect(),
                                 pattern.azure_color_pattern,
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
                                   &start.as_azure_point(),
                                   &end.as_azure_point(),
                                   pattern.azure_color_pattern,
                                   &stroke_options.as_azure_stroke_options(),
                                   &draw_options.as_azure_draw_options());
        }
    }

    pub fn stroke_rect(&self,
                   rect: &Rect<AzFloat>,
                   pattern: &ColorPattern,
                   stroke_options: &StrokeOptions,
                   draw_options: &DrawOptions) {
        unsafe {
            AzDrawTargetStrokeRect(self.azure_draw_target,
                                   &rect.as_azure_rect(),
                                   pattern.azure_color_pattern,
                                   &stroke_options.as_azure_stroke_options(),
                                   &draw_options.as_azure_draw_options());
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
                                    &dest.as_azure_rect(),
                                    &source.as_azure_rect(),
                                    &surf_options.as_azure_draw_surface_options(),
                                    &options.as_azure_draw_options());
        }
    }

    pub fn snapshot(&self) -> SourceSurface {
        unsafe {
            let azure_surface = AzDrawTargetGetSnapshot(self.azure_draw_target);
            SourceSurface(azure_surface)
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
                &size.as_azure_int_size(),
                stride,
                format.as_azure_surface_format());
            SourceSurface(azure_surface)
        }
    }

    pub fn set_transform(&self, matrix: &Matrix2D<AzFloat>) {
        unsafe {
            AzDrawTargetSetTransform(self.azure_draw_target, cast::transmute(matrix));
        }
    }

    pub fn fill_glyphs(&self, azfontref: AzScaledFontRef, glyphbuf: struct__AzGlyphBuffer,
                              azure_pattern: AzColorPatternRef,
                              options: struct__AzDrawOptions,
                              renderingOptions: AzGlyphRenderingOptionsRef) {
        unsafe {
            AzDrawTargetFillGlyphs(self.azure_draw_target,
                    azfontref,
                    &glyphbuf,
                    azure_pattern,
                    &options,
                    renderingOptions);
        }
    }

    pub fn create_path_builder(&self) -> PathBuilder {
        unsafe {
            PathBuilder {
                azure_path_builder: AzCreatePathBuilder(self.azure_draw_target)
            }
        }
    }

    pub fn push_clip(&self, path: &Path) {
        unsafe {
            AzDrawTargetPushClip(self.azure_draw_target,path.azure_path);
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
    azure_source_surface: AzSourceSurfaceRef,
}

impl Drop for SourceSurface {
    fn drop(&mut self) {
        unsafe {
            AzReleaseSourceSurface(self.azure_source_surface);
        }
    }
}

pub fn SourceSurface(azure_source_surface: AzSourceSurfaceRef) -> SourceSurface {
    SourceSurface {
        azure_source_surface: azure_source_surface
    }
}

// FIXME Rust #8753 no fixed stack segment for default methods
unsafe fn AzSourceSurfaceGetSize_(aSurface: AzSourceSurfaceRef) -> AzIntSize {
    AzSourceSurfaceGetSize(aSurface)
}

// FIXME Rust #8753 no fixed stack segment for default methods
unsafe fn AzSourceSurfaceGetFormat_(aSurface: AzSourceSurfaceRef) -> AzSurfaceFormat {
    AzSourceSurfaceGetFormat(aSurface)
}

pub trait SourceSurfaceMethods {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef;

    fn size(&self) -> Size2D<i32> {
        unsafe {
            let size = AzSourceSurfaceGetSize_(self.get_azure_source_surface());
            Size2D { width: size.width, height: size.height }
        }
    }

    fn format(&self) -> SurfaceFormat {
        unsafe {
            SurfaceFormat::new(AzSourceSurfaceGetFormat_(self.get_azure_source_surface()))
        }
    }
}

impl SourceSurface {
    pub fn get_data_surface(&self) -> DataSourceSurface {
        unsafe {
            let data_source_surface = AzSourceSurfaceGetDataSurface(
                self.azure_source_surface);
            DataSourceSurface {
                azure_data_source_surface: data_source_surface
            }
        }
    }
}

impl SourceSurfaceMethods for SourceSurface {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef { self.azure_source_surface }
}

pub struct DataSourceSurface {
    azure_data_source_surface: AzDataSourceSurfaceRef,
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
            let buf = AzDataSourceSurfaceGetData(self.azure_data_source_surface);
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
    azure_path: AzPathRef
}

impl Drop for Path {
    fn drop(&mut self){
        unsafe {
            AzReleasePath(self.azure_path);
        }
    }
}

pub struct PathBuilder {
    azure_path_builder: AzPathBuilderRef
}

impl PathBuilder {
    pub fn move_to(&self, point: Point2D<AzFloat>) {
        unsafe {
            let az_point = point.as_azure_point();
            AzPathBuilderMoveTo(self.azure_path_builder, &az_point);
        }
    }

    pub fn line_to(&self, point: Point2D<AzFloat>) {
        unsafe {
            let az_point = point.as_azure_point();
            AzPathBuilderLineTo(self.azure_path_builder, &az_point);
        }
    }

    pub fn finish(&self) -> Path{
        unsafe {
            let az_path = AzPathBuilderFinish(self.azure_path_builder);
            Path {
                azure_path : az_path
            }
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

pub fn current_gl_context() -> AzGLContext {
    unsafe {
        AzSkiaGetCurrentGLContext()
    }
}

#[cfg(target_os="linux")]
pub fn current_display() -> *c_void {
    use glfw;
    unsafe {
        glfw::ffi::glfwGetX11Display()
    }
}

#[cfg(target_os="linux")]
pub fn current_graphics_metadata() -> NativeGraphicsMetadata {
    NativeGraphicsMetadata {
        display: current_display(),
    }
}

#[cfg(target_os="macos")]
pub fn current_graphics_metadata() -> NativeGraphicsMetadata {
    use opengles::cgl::{CGLGetCurrentContext, CGLGetPixelFormat};
    unsafe {
        NativeGraphicsMetadata {
            pixel_format: CGLGetPixelFormat(CGLGetCurrentContext()),
        }
    }
}

#[cfg(target_os="android")]
pub fn current_graphics_metadata() -> NativeGraphicsMetadata {
    use egl::egl::GetCurrentDisplay;
    NativeGraphicsMetadata {
        display: GetCurrentDisplay(),
    }
}

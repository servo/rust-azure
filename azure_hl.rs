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

use extra::arc::Arc;
use geom::matrix2d::Matrix2D;
use geom::point::Point2D;
use geom::rect::Rect;
use geom::size::Size2D;
use gl = opengles::gl2;
use layers::platform::surface::{NativeGraphicsMetadata, NativePaintingGraphicsContext};
use std::libc::types::common::c99::{uint8_t, uint16_t};
use std::libc::size_t;
use std::cast;
use std::ptr;
use std::ptr::{null, to_unsafe_ptr};
use std::vec;

#[cfg(target_os="linux")]
use std::libc::c_void;

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
    #[fixed_stack_segment]
    fn drop(&mut self) {
        unsafe {
            AzReleaseColorPattern(self.azure_color_pattern);
        }
    }
}

#[fixed_stack_segment]
pub fn ColorPattern(color: Color) -> ColorPattern {
    unsafe {
        ColorPattern {
            azure_color_pattern: AzCreateColorPattern(&color.as_azure_color())
        }
    }
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
    fn as_azure_filter(self) -> AzFilter {
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
    #[fixed_stack_segment]
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
    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
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
    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
    pub fn get_size(&self) -> AzIntSize {
        unsafe {
            AzDrawTargetGetSize(self.azure_draw_target)
        }
    }

    #[fixed_stack_segment]
    pub fn flush(&self) {
        unsafe {
            AzDrawTargetFlush(self.azure_draw_target);
            match self.skia_context {
                None => {}
                Some(ctx) => { AzSkiaSharedGLContextFlush(ctx); }
            }
        }
    }

    #[fixed_stack_segment]
    pub fn clear_rect(&self, rect: &Rect<AzFloat>) {
        unsafe {
            AzDrawTargetClearRect(self.azure_draw_target, &rect.as_azure_rect());
        }
    }

    #[fixed_stack_segment]
    pub fn fill_rect(&self, rect: &Rect<AzFloat>, pattern: &ColorPattern) {
        unsafe {
            AzDrawTargetFillRect(self.azure_draw_target,
                                 to_unsafe_ptr(&rect.as_azure_rect()),
                                 pattern.azure_color_pattern);
        }
    }

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
    pub fn draw_surface(&self,
                    surface: SourceSurface,
                    dest: Rect<AzFloat>,
                    source: Rect<AzFloat>,
                    surf_options: DrawSurfaceOptions,
                    options: DrawOptions) {
        unsafe {
            AzDrawTargetDrawSurface(self.azure_draw_target,
                                    surface.azure_source_surface,
                                    to_unsafe_ptr(&dest.as_azure_rect()),
                                    to_unsafe_ptr(&source.as_azure_rect()),
                                    to_unsafe_ptr(&surf_options.as_azure_draw_surface_options()),
                                    to_unsafe_ptr(&options.as_azure_draw_options()));
        }
    }

    #[fixed_stack_segment]
    pub fn snapshot(&self) -> SourceSurface {
        unsafe {
            let azure_surface = AzDrawTargetGetSnapshot(self.azure_draw_target);
            SourceSurface(azure_surface)
        }
    }

    #[fixed_stack_segment]
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

    #[fixed_stack_segment]
    pub fn set_transform(&self, matrix: &Matrix2D<AzFloat>) {
        unsafe {
            AzDrawTargetSetTransform(self.azure_draw_target, cast::transmute(matrix));
        }
    }

    #[fixed_stack_segment]
    pub fn fill_glyphs(&self, azfontref: AzScaledFontRef, glyphbuf: struct__AzGlyphBuffer,
                              azure_pattern: AzColorPatternRef,
                              options: struct__AzDrawOptions,
                              renderingOptions: AzGlyphRenderingOptionsRef) {
        unsafe {
            AzDrawTargetFillGlyphs(self.azure_draw_target,
                    azfontref,
                    to_unsafe_ptr(&glyphbuf),
                    azure_pattern,
                    to_unsafe_ptr(&options),
                    renderingOptions);
        }
    }
}

// Ugly workaround for the lack of explicit self.
pub fn clone_mutable_draw_target(draw_target: &mut DrawTarget) -> DrawTarget {
    return draw_target.clone();
}

pub struct SourceSurface {
    priv azure_source_surface: AzSourceSurfaceRef,
}

impl Drop for SourceSurface {
    #[fixed_stack_segment]
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
#[fixed_stack_segment]
unsafe fn AzSourceSurfaceGetSize_(aSurface: AzSourceSurfaceRef) -> AzIntSize {
    AzSourceSurfaceGetSize(aSurface)
}

// FIXME Rust #8753 no fixed stack segment for default methods
#[fixed_stack_segment]
unsafe fn AzSourceSurfaceGetFormat_(aSurface: AzSourceSurfaceRef) -> AzSurfaceFormat {
    AzSourceSurfaceGetFormat(aSurface)
}

pub trait SourceSurfaceMethods {
    fn get_azure_source_surface(&self) -> AzSourceSurfaceRef;

    #[fixed_stack_segment]
    fn size(&self) -> Size2D<i32> {
        unsafe {
            let size = AzSourceSurfaceGetSize_(self.get_azure_source_surface());
            Size2D { width: size.width, height: size.height }
        }
    }

    #[fixed_stack_segment]
    fn format(&self) -> SurfaceFormat {
        unsafe {
            SurfaceFormat::new(AzSourceSurfaceGetFormat_(self.get_azure_source_surface()))
        }
    }
}

impl SourceSurface {
    #[fixed_stack_segment]
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
    priv azure_data_source_surface: AzDataSourceSurfaceRef,
}

impl Drop for DataSourceSurface {
    #[fixed_stack_segment]
    fn drop(&mut self) {
        unsafe {
            AzReleaseSourceSurface(self.azure_data_source_surface);
        }
    }
}

impl DataSourceSurface {
    #[fixed_stack_segment]
    pub fn with_data(&self, f: &fn(&[u8])) {
        unsafe {
            let buf = AzDataSourceSurfaceGetData(self.azure_data_source_surface);
            let len = self.stride() * self.size().height;
            vec::raw::buf_as_slice(buf, len as uint, f);
        }
    }

    #[fixed_stack_segment]
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

#[fixed_stack_segment]
pub fn current_gl_context() -> AzGLContext {
    unsafe {
        AzSkiaGetCurrentGLContext()
    }
}

#[cfg(target_os="linux")]
#[fixed_stack_segment]
pub fn current_display() -> *c_void {
    use glfw;
    unsafe {
        glfw::ffi::glfwGetX11Display()
    }
}

#[cfg(target_os="linux")]
#[fixed_stack_segment]
pub fn current_graphics_metadata() -> NativeGraphicsMetadata {
    use xlib::xlib::XDisplayString;
    unsafe {
        XDisplayString(current_display())
    }
}

#[cfg(target_os="macos")]
#[cfg(target_os="android")]
#[fixed_stack_segment]
pub fn current_graphics_metadata() -> NativeGraphicsMetadata {
    use opengles::cgl::{CGLGetCurrentContext, CGLGetPixelFormat};
    unsafe {
        NativeGraphicsMetadata {
            pixel_format: CGLGetPixelFormat(CGLGetCurrentContext()),
        }
    }
}


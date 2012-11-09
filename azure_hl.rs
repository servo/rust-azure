// High-level bindings to Azure.
use cairo_hl = cairo::cairo_hl;

use bindgen::{AzCreateColorPattern, AzCreateDrawTarget, AzCreateDrawTargetForCairoSurface};
use bindgen::{AzCreateDrawTargetForData, AzDataSourceSurfaceGetData, AzDataSourceSurfaceGetStride};
use bindgen::AzDrawTargetClearRect;
use bindgen::AzDrawTargetCreateSourceSurfaceFromData;
use bindgen::{AzDrawTargetDrawSurface, AzDrawTargetFillRect, AzDrawTargetFlush};
use bindgen::{AzDrawTargetGetSnapshot, AzDrawTargetSetTransform, AzDrawTargetStrokeRect};
use bindgen::{AzReleaseColorPattern, AzReleaseDrawTarget};
use bindgen::AzReleaseSourceSurface;
use bindgen::AzRetainDrawTarget;
use bindgen::{AzSourceSurfaceGetDataSurface, AzSourceSurfaceGetFormat, AzSourceSurfaceGetSize};
pub use cairo_hl::ImageSurface;
use geom::matrix2d::Matrix2D;
use geom::rect::Rect;
use geom::size::Size2D;
use ptr::{null, to_unsafe_ptr};
use cast::transmute;

pub trait AsAzureRect {
    fn as_azure_rect() -> AzRect;
}

impl Rect<AzFloat> : AsAzureRect {
    fn as_azure_rect() -> AzRect {
        {
            x: self.origin.x,
            y: self.origin.y,
            width: self.size.width,
            height: self.size.height
        }
    }
}

pub trait AsAzureIntSize {
    fn as_azure_int_size() -> AzIntSize;
}

impl Size2D<i32> : AsAzureIntSize {
    fn as_azure_int_size() -> AzIntSize {
        {
            width: self.width,
            height: self.height
        }
    }
}

pub struct Color {
    r: AzFloat,
    g: AzFloat,
    b: AzFloat,
    a: AzFloat,
}

impl Color {
    fn as_azure_color() -> AzColor {
        { r: self.r, g: self.g, b: self.b, a: self.a }
    }
}

pub fn Color(r: AzFloat, g: AzFloat, b: AzFloat, a: AzFloat) -> Color {
    Color { r: r, g: g, b: b, a: a }
}


// FIXME: Should have a class hierarchy here starting with Pattern.
pub struct ColorPattern {
    azure_color_pattern: AzColorPatternRef,

    drop {
        AzReleaseColorPattern(self.azure_color_pattern);
    }
}

pub fn ColorPattern(color: Color) -> ColorPattern {
    ColorPattern { 
        azure_color_pattern: AzCreateColorPattern(to_unsafe_ptr(&color.as_azure_color()))
    }
}

pub struct StrokeOptions {
    line_width: AzFloat,
    miter_limit: AzFloat,
    fields: uint16_t,
}

impl StrokeOptions {
    fn as_azure_stroke_options() -> AzStrokeOptions {
        {
            mLineWidth: self.line_width,
            mMiterLimit: self.miter_limit,
            mDashPattern: null(),
            mDashLength: 0,
            mDashOffset: 0.0f as AzFloat,
            fields: self.fields
        }
    }
}

pub fn StrokeOptions(line_width: AzFloat,
                  miter_limit: AzFloat,
                  fields: uint16_t) -> StrokeOptions {
    StrokeOptions {
        line_width: line_width,
        miter_limit: miter_limit,
        fields: fields
    }
}

pub struct DrawOptions {
    alpha: AzFloat,
    fields: uint16_t,
}

impl DrawOptions {
    fn as_azure_draw_options() -> AzDrawOptions {
        {
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
    R8G8R8X8,
    R5G6B5,
    A8
}

impl SurfaceFormat {
    fn as_azure_surface_format() -> AzSurfaceFormat {
        self as AzSurfaceFormat
    }

    static pub fn new(azure_surface_format: AzSurfaceFormat) -> SurfaceFormat {
        match azure_surface_format {
            0 => B8G8R8A8,
            1 => R8G8R8X8,
            2 => R5G6B5,
            3 => A8,
            _ => fail ~"SurfaceFormat::new(): unknown Azure surface format"
        }
    }
}

pub enum Filter {
    Linear,
    Point
}

impl Filter {
    fn as_azure_filter() -> AzFilter {
        self as AzFilter
    }
}

pub struct DrawSurfaceOptions {
    filter: Filter,
    sampling_bounds: bool,
}

impl DrawSurfaceOptions {
    fn as_azure_draw_surface_options() -> AzDrawSurfaceOptions {
        { fields: ((self.filter as int) | (if self.sampling_bounds { 8 } else { 0 })) as u32 }
    }
}


pub fn DrawSurfaceOptions(filter: Filter, sampling_bounds: bool) -> DrawSurfaceOptions {
    DrawSurfaceOptions {
        filter: filter,
        sampling_bounds: sampling_bounds,
    }
}


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
    data: Option<~[u8]>,

    drop {
        AzReleaseDrawTarget(self.azure_draw_target);
    }
}

pub impl DrawTarget {
    static pub fn new(backend: BackendType, size: Size2D<i32>, format: SurfaceFormat)
                   -> DrawTarget {
        let azure_draw_target = AzCreateDrawTarget(backend.as_azure_backend_type(),
                                                   to_unsafe_ptr(&size.as_azure_int_size()),
                                                   format.as_azure_surface_format());
        if azure_draw_target == ptr::null() { fail; }
        DrawTarget { azure_draw_target: move azure_draw_target, data: None }
    }

    static pub fn new_with_data(backend: BackendType,
                                data: ~[u8],
                                offset: uint,
                                size: Size2D<i32>,
                                stride: i32,
                                format: SurfaceFormat) -> DrawTarget {
        assert (data.len() - offset) as i32 >= stride * size.height;
        let azure_draw_target =
            AzCreateDrawTargetForData(backend.as_azure_backend_type(),
                                      to_unsafe_ptr(&data[offset]),
                                      to_unsafe_ptr(&size.as_azure_int_size()),
                                      stride,
                                      format.as_azure_surface_format());
        if azure_draw_target == ptr::null() { fail; }
        DrawTarget { azure_draw_target: move azure_draw_target, data: Some(move data) }
    }

    fn clone() -> DrawTarget {
        return new_draw_target_from_azure_draw_target(self.azure_draw_target);
    }

    fn flush() {
        AzDrawTargetFlush(self.azure_draw_target);
    }

    fn clear_rect(rect: &Rect<AzFloat>) {
        AzDrawTargetClearRect(self.azure_draw_target, to_unsafe_ptr(&rect.as_azure_rect()));
    }

    fn fill_rect(rect: &Rect<AzFloat>, pattern: &ColorPattern) {
        AzDrawTargetFillRect(self.azure_draw_target,
                             to_unsafe_ptr(&rect.as_azure_rect()),
                             pattern.azure_color_pattern);
    }

    fn stroke_rect(rect: &Rect<AzFloat>, pattern: &ColorPattern, stroke_options: &StrokeOptions,
                   draw_options: &DrawOptions) {
        AzDrawTargetStrokeRect(self.azure_draw_target,
                               to_unsafe_ptr(&rect.as_azure_rect()),
                               pattern.azure_color_pattern,
                               to_unsafe_ptr(&stroke_options.as_azure_stroke_options()),
                               to_unsafe_ptr(&draw_options.as_azure_draw_options()));
    }

    fn draw_surface(surface: SourceSurface, dest: Rect<AzFloat>, source: Rect<AzFloat>,
                    surf_options: DrawSurfaceOptions, options: DrawOptions) {
        AzDrawTargetDrawSurface(self.azure_draw_target,
                                surface.azure_source_surface,
                                to_unsafe_ptr(&dest.as_azure_rect()),
                                to_unsafe_ptr(&source.as_azure_rect()),
                                to_unsafe_ptr(&surf_options.as_azure_draw_surface_options()),
                                to_unsafe_ptr(&options.as_azure_draw_options()));
    }

    fn snapshot(&const self) -> SourceSurface {
        let azure_surface = AzDrawTargetGetSnapshot(self.azure_draw_target);
        SourceSurface(azure_surface)
    }

    fn create_source_surface_from_data(data: &[u8], size: Size2D<i32>, stride: i32,
                                       format: SurfaceFormat) -> SourceSurface {

        assert data.len() as i32 == stride * size.height;
        let azure_surface =
            AzDrawTargetCreateSourceSurfaceFromData(self.azure_draw_target,
                                                    to_unsafe_ptr(&data[0]),
                                                    to_unsafe_ptr(&size.as_azure_int_size()),
                                                    stride,
                                                    format.as_azure_surface_format());
        SourceSurface(azure_surface)
    }

    fn set_transform(matrix: &Matrix2D<AzFloat>) {
        unsafe {
            AzDrawTargetSetTransform(self.azure_draw_target, transmute(matrix));
        }
    }
}


pub fn DrawTarget(cairo_surface: &ImageSurface) -> DrawTarget {
	let size = { width: cairo_surface.width(), height: cairo_surface.height() };
    DrawTarget {
        azure_draw_target: AzCreateDrawTargetForCairoSurface(cairo_surface.cairo_surface,
														     to_unsafe_ptr(&size)),
        data: None
    }
}


// Ugly workaround for the lack of explicit self.
pub fn clone_mutable_draw_target(draw_target: &mut DrawTarget) -> DrawTarget {
    return new_draw_target_from_azure_draw_target(draw_target.azure_draw_target);
}

pub fn new_draw_target(cairo_surface: &ImageSurface) -> DrawTarget {
	let size = { width: cairo_surface.width(), height: cairo_surface.height() };
    DrawTarget {
        azure_draw_target: AzCreateDrawTargetForCairoSurface(cairo_surface.cairo_surface,
														     to_unsafe_ptr(&size)),
        data: None
    }
}

pub fn new_draw_target_from_azure_draw_target(azure_draw_target: AzDrawTargetRef) -> DrawTarget {
    AzRetainDrawTarget(azure_draw_target);
    DrawTarget {
        azure_draw_target: azure_draw_target,
        data: None
    }
}

pub struct SourceSurface {
    priv azure_source_surface: AzSourceSurfaceRef,

    drop {
        AzReleaseSourceSurface(self.azure_source_surface);
    }
}

pub fn SourceSurface(azure_source_surface: AzSourceSurfaceRef) -> SourceSurface {
    SourceSurface {
        azure_source_surface: azure_source_surface
    }
}

pub trait SourceSurfaceMethods {
    fn get_azure_source_surface(&const self) -> AzSourceSurfaceRef;

    fn size(&const self) -> Size2D<i32> {
        let size = AzSourceSurfaceGetSize(self.get_azure_source_surface());
        Size2D { width: size.width, height: size.height }
    }

    fn format(&const self) -> SurfaceFormat {
        SurfaceFormat::new(AzSourceSurfaceGetFormat(self.get_azure_source_surface()))
    }
}

impl SourceSurface {
    pub fn get_data_surface(&const self) -> DataSourceSurface {
        DataSourceSurface {
            azure_data_source_surface: AzSourceSurfaceGetDataSurface(self.azure_source_surface)
        }
    }
}

impl SourceSurface : SourceSurfaceMethods {
    fn get_azure_source_surface(&const self) -> AzSourceSurfaceRef { self.azure_source_surface }
}

pub struct DataSourceSurface {
    priv azure_data_source_surface: AzDataSourceSurfaceRef,

    drop {
        AzReleaseSourceSurface(self.azure_data_source_surface);
    }
}

impl DataSourceSurface {
    pub fn with_data(&const self, f: &fn(&[u8])) {
        unsafe {
            let buf = AzDataSourceSurfaceGetData(self.azure_data_source_surface);
            let len = self.stride() * self.size().height;
            vec::raw::buf_as_slice(buf, len as uint, f);
        }
    }

    pub fn stride(&const self) -> i32 {
        AzDataSourceSurfaceGetStride(self.azure_data_source_surface)
    }

    // FIXME: Workaround for lack of working cross-crate default methods.
    pub fn get_size(&const self) -> Size2D<i32> {
        self.size()
    }
}

impl DataSourceSurface : SourceSurfaceMethods {
    fn get_azure_source_surface(&const self) -> AzSourceSurfaceRef {
        self.azure_data_source_surface
    }
}


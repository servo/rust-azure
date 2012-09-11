// High-level bindings to Azure.

use bindgen::{AzCreateColorPattern, AzCreateDrawTargetForCairoSurface};
//use bindgen::AzDrawSurfaceOptions;
use bindgen::AzDrawTargetClearRect;
use bindgen::AzDrawTargetCreateSourceSurfaceFromData;
use bindgen::{AzDrawTargetDrawSurface, AzDrawTargetFillRect, AzDrawTargetFlush};
use bindgen::{AzDrawTargetStrokeRect};
//use bindgen::AzFilter;
use bindgen::{AzReleaseColorPattern, AzReleaseDrawTarget};
use bindgen::AzReleaseSourceSurface;
use bindgen::AzRetainDrawTarget;
//use bindgen::AzSurfaceFormat;
use cairo_hl::ImageSurface;
use geom::rect::Rect;
use geom::size::Size2D;
use ptr::{addr_of, null};

trait AsAzureRect {
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

trait AsAzureIntSize {
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

struct Color {
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

fn Color(r: AzFloat, g: AzFloat, b: AzFloat, a: AzFloat) -> Color {
    Color { r: r, g: g, b: b, a: a }
}


// FIXME: Should have a class hierarchy here starting with Pattern.
struct ColorPattern {
    azure_color_pattern: AzColorPatternRef,

    drop {
        AzReleaseColorPattern(self.azure_color_pattern);
    }
}

fn ColorPattern(color: Color) -> ColorPattern {
    ColorPattern { 
        azure_color_pattern: AzCreateColorPattern(addr_of(color.as_azure_color()))
    }
}

struct StrokeOptions {
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
            fields: 0
        }
    }
}

fn StrokeOptions(line_width: AzFloat,
                  miter_limit: AzFloat,
                  fields: uint16_t) -> StrokeOptions {
    StrokeOptions {
        line_width: line_width,
        miter_limit: miter_limit,
        fields: fields
    }
}

struct DrawOptions {
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


fn DrawOptions(alpha: AzFloat, fields: uint16_t) -> DrawOptions {
    DrawOptions {
        alpha : alpha,
        fields : fields,
    }
}

enum SurfaceFormat {
    B8G8R8A8,
    R8G8R8X8,
    R5G6B5,
    A8
}

impl SurfaceFormat {
    fn as_azure_surface_format() -> AzSurfaceFormat {
        self as AzSurfaceFormat
    }
}

enum Filter {
    Linear,
    Point
}

impl Filter {
    fn as_azure_filter() -> AzFilter {
        self as AzFilter
    }
}

struct DrawSurfaceOptions {
    filter: Filter,
    sampling_bounds: bool,
}

impl DrawSurfaceOptions {
    fn as_azure_draw_surface_options() -> AzDrawSurfaceOptions {
        { fields: ((self.filter as int) | (if self.sampling_bounds { 8 } else { 0 })) as u32 }
    }
}


fn DrawSurfaceOptions(filter: Filter, sampling_bounds: bool) -> DrawSurfaceOptions {
    DrawSurfaceOptions {
        filter: filter,
        sampling_bounds: sampling_bounds,
    }
}


struct DrawTarget {
    azure_draw_target: AzDrawTargetRef,

    drop {
        AzReleaseDrawTarget(self.azure_draw_target);
    }
}

impl DrawTarget {
    fn clone() -> DrawTarget {
        return new_draw_target_from_azure_draw_target(self.azure_draw_target);
    }

    fn flush() {
        AzDrawTargetFlush(self.azure_draw_target);
    }

    fn clear_rect(&&rect: Rect<AzFloat>) {
        AzDrawTargetClearRect(self.azure_draw_target, addr_of(rect.as_azure_rect()));
    }

    fn fill_rect(&&rect: Rect<AzFloat>, &&pattern: ColorPattern) {
        AzDrawTargetFillRect(self.azure_draw_target,
                             addr_of(rect.as_azure_rect()),
                             pattern.azure_color_pattern);
    }

    fn stroke_rect(&&rect: Rect<AzFloat>, &&pattern: ColorPattern, &&stroke_options: StrokeOptions,
                   &&draw_options: DrawOptions) {
        AzDrawTargetStrokeRect(self.azure_draw_target,
                               addr_of(rect.as_azure_rect()),
                               pattern.azure_color_pattern,
                               addr_of(stroke_options.as_azure_stroke_options()),
                               addr_of(draw_options.as_azure_draw_options()));
    }

    fn draw_surface(surface: SourceSurface, dest: Rect<AzFloat>, source: Rect<AzFloat>,
                    surf_options: DrawSurfaceOptions, options: DrawOptions) {

        AzDrawTargetDrawSurface(self.azure_draw_target,
                                surface.azure_source_surface,
                                addr_of(dest.as_azure_rect()),
                                addr_of(source.as_azure_rect()),
                                addr_of(surf_options.as_azure_draw_surface_options()),
                                addr_of(options.as_azure_draw_options()));
    }

    fn create_source_surface_from_data(data: &[u8], size: Size2D<i32>, stride: i32,
                                       format: SurfaceFormat) -> SourceSurface {

        assert data.len() as i32 == stride * size.height;
        let azure_surface =
            AzDrawTargetCreateSourceSurfaceFromData(self.azure_draw_target,
                                                    addr_of(data[0]),
                                                    addr_of(size.as_azure_int_size()),
                                                    stride,
                                                    format.as_azure_surface_format());
        SourceSurface(azure_surface)
    }
}


fn DrawTarget(&&cairo_surface: ImageSurface) -> DrawTarget {
    DrawTarget {
        azure_draw_target: AzCreateDrawTargetForCairoSurface(cairo_surface.cairo_surface)
    }
}


// Ugly workaround for the lack of explicit self.
fn clone_mutable_draw_target(draw_target: &mut DrawTarget) -> DrawTarget {
    return new_draw_target_from_azure_draw_target(draw_target.azure_draw_target);
}

fn new_draw_target(cairo_surface: &ImageSurface) -> DrawTarget {
    DrawTarget {
        azure_draw_target: AzCreateDrawTargetForCairoSurface(cairo_surface.cairo_surface)
    }
}

fn new_draw_target_from_azure_draw_target(azure_draw_target: AzDrawTargetRef) -> DrawTarget {
    AzRetainDrawTarget(azure_draw_target);
    DrawTarget {
        azure_draw_target: azure_draw_target
    }
}

struct SourceSurface {
    azure_source_surface: AzSourceSurfaceRef,

    drop {
        AzReleaseSourceSurface(self.azure_source_surface);
    }
}

fn SourceSurface(azure_source_surface: AzSourceSurfaceRef) -> SourceSurface {
    return SourceSurface {
        azure_source_surface:  azure_source_surface
    }
}


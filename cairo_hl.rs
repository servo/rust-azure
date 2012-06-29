// High-level bindings to Cairo.

import cairo::cairo_surface_t;
import cairo::bindgen::{cairo_create, cairo_fill, cairo_image_surface_create};
import cairo::bindgen::{cairo_image_surface_get_data, cairo_image_surface_get_format};
import cairo::bindgen::{cairo_image_surface_get_height, cairo_image_surface_get_stride};
import cairo::bindgen::{cairo_image_surface_get_width, cairo_rectangle, cairo_set_line_width};
import cairo::bindgen::{cairo_set_source_rgb, cairo_stroke, cairo_surface_destroy};
import vec::unsafe::from_buf;

// FIXME: We should have a hierarchy of surfaces, but this needs to wait on case classes.
class ImageSurface {
    let cairo_surface: *cairo_surface_t;

    new(format: cairo_format_t, width: c_int, height: c_int) {
        self.cairo_surface = cairo_image_surface_create(format, width, height);
        if self.cairo_surface.is_null() {
            fail "couldn't create Cairo image surface";
        };
    }

    fn width()  -> c_int    { cairo_image_surface_get_width(self.cairo_surface)  }
    fn height() -> c_int    { cairo_image_surface_get_height(self.cairo_surface) }
    fn stride() -> c_int    { cairo_image_surface_get_stride(self.cairo_surface) }
    fn format() -> c_int    { cairo_image_surface_get_format(self.cairo_surface) }

    fn data() -> [u8]/~ unsafe {
        let buffer = cairo_image_surface_get_data(self.cairo_surface);
        ret from_buf(buffer, (self.stride() * self.height()) as uint);
    }

    drop {
        cairo_surface_destroy(self.cairo_surface);
    }
}

class Context {
    let cairo_context: *cairo_t;

    new(&&surface: ImageSurface) {
        self.cairo_context = cairo_create(surface.cairo_surface);
    }

    fn set_line_width(width: c_double) {
        cairo_set_line_width(self.cairo_context, width);
    }

    fn set_source_rgb(r: c_double, g: c_double, b: c_double) {
        cairo_set_source_rgb(self.cairo_context, r, g, b);
    }

    fn rectangle(x: c_double, y: c_double, width: c_double, height: c_double) {
        cairo_rectangle(self.cairo_context, x, y, width, height);
    }

    fn stroke() {
        cairo_stroke(self.cairo_context);
    }

    fn fill() {
        cairo_fill(self.cairo_context);
    }
}


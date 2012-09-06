// High-level bindings to Cairo.

import cairo::{CAIRO_STATUS_SUCCESS, cairo_format_t, cairo_status_t, cairo_surface_t, cairo_t};
import cairo::bindgen::{cairo_create, cairo_fill, cairo_image_surface_create};
import cairo::bindgen::{cairo_image_surface_get_data, cairo_image_surface_get_format};
import cairo::bindgen::{cairo_image_surface_get_height, cairo_image_surface_get_stride};
import cairo::bindgen::{cairo_image_surface_get_width, cairo_rectangle, cairo_set_line_width};
import cairo::bindgen::{cairo_set_source_rgb, cairo_stroke, cairo_surface_destroy};
import cairo::bindgen::{cairo_surface_reference, cairo_surface_write_to_png_stream};
import io::{MemBuffer, Writer};
import ptr::addr_of;
import result::{Err, Ok, Result};
import unsafe::reinterpret_cast;
import vec::unsafe::{form_slice, from_buf};

// FIXME: We should have a hierarchy of surfaces, but this needs to wait on case classes.
struct ImageSurface {
    let cairo_surface: *cairo_surface_t;

    fn width()  -> c_int    { cairo_image_surface_get_width(self.cairo_surface)  }
    fn height() -> c_int    { cairo_image_surface_get_height(self.cairo_surface) }
    fn stride() -> c_int    { cairo_image_surface_get_stride(self.cairo_surface) }
    fn format() -> c_int    { cairo_image_surface_get_format(self.cairo_surface) }

    // FIXME: This should not copy!
    pure fn data() -> ~[u8] unsafe {
        let buffer = cairo_image_surface_get_data(self.cairo_surface);
        return from_buf(buffer, (self.stride() * self.height()) as uint);
    }

    drop {
        cairo_surface_destroy(self.cairo_surface);
    }
}

// Should be private.
fn image_surface_from_cairo_surface(cairo_surface: *cairo_surface_t) -> ImageSurface {
    assert !cairo_surface.is_null();
    ImageSurface { cairo_surface: cairo_surface }
}

fn ImageSurface(format: cairo_format_t, width: c_int, height: c_int) -> ImageSurface {
    let cairo_surface = cairo_image_surface_create(format, width, height);
    if cairo_surface.is_null() {
        fail ~"couldn't create Cairo image surface";
    }
    return image_surface_from_cairo_surface(move cairo_surface);
}

impl ImageSurface {
    fn write_to_png_stream(buffer: &io::MemBuffer) -> Result<(),cairo_status_t> unsafe {
        extern fn write_fn(closure: *c_void, data: *c_uchar, len: c_uint)
                        -> cairo_status_t unsafe {
            let writer: *MemBuffer = reinterpret_cast(&closure);
            do form_slice(data, len as uint) |bytes| {
                (*writer).write(bytes);
            }
            return CAIRO_STATUS_SUCCESS;
        }

        let buffer_ptr = reinterpret_cast(&buffer);
        let status = cairo_surface_write_to_png_stream(self.cairo_surface, write_fn, buffer_ptr);
        if status != CAIRO_STATUS_SUCCESS {
            return Err(status);
        }

        return Ok(());
    }

    fn clone() -> ImageSurface {
        cairo_surface_reference(self.cairo_surface);
        return image_surface_from_cairo_surface(self.cairo_surface);
    }
}

struct Context {
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


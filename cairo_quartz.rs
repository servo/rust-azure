// FIXME: Use bindgen

extern mod core_graphics;

use core_graphics::font::CGFontRef;

#[nolink]
pub extern mod bindgen {
    fn cairo_quartz_font_face_create_for_cgfont(font: CGFontRef) -> *cairo::cairo_font_face_t;
}

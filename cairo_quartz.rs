// FIXME: Use bindgen

use cocoa;

import cocoa::cg::CGFontRef;

#[nolink]
extern mod bindgen {
    fn cairo_quartz_font_face_create_for_cgfont(font: CGFontRef) -> *cairo::cairo_font_face_t;
}

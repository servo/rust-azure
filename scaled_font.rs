// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#[cfg(cairo)]
extern mod cairo;

use azure::{AzScaledFontRef, AzFloat};
use azure::{AZ_NATIVE_FONT_CAIRO_FONT_FACE, AZ_NATIVE_FONT_MAC_FONT_FACE};
use azure::{AZ_FONT_STYLE_NORMAL, AZ_NATIVE_FONT_SKIA_FONT_FACE};
use azure::{struct__AzNativeFont};

use azure_hl::{BackendType, CairoBackend, CoreGraphicsAcceleratedBackend};
use azure_hl::{CoreGraphicsBackend, Direct2DBackend, NoBackend, RecordingBackend};
use azure_hl::{SkiaBackend};
use azure::bindgen::{AzCreateScaledFontForNativeFont, AzReleaseScaledFont};
use azure::bindgen::{AzCreateFontOptions, AzDestroyFontOptions};

use core::libc::{c_void, c_double, c_int};

#[cfg(target_os="macos")]
priv use scaled_font::macos::*;

#[cfg(target_os="linux")]
priv use scaled_font::linux::*;

#[cfg(cairo)]
priv use scaled_font::cairo::*;

#[cfg(cairo)]
mod cairo {
    pub use cairo::cairo::{struct__cairo_matrix};
    pub use cairo::cairo::bindgen::{cairo_font_face_destroy, cairo_font_options_create};
    pub use cairo::cairo::bindgen::{cairo_font_options_destroy, cairo_matrix_init_identity};
    pub use cairo::cairo::bindgen::{cairo_matrix_scale, cairo_scaled_font_create};
}

#[cfg(target_os="macos")]
pub mod macos {
    extern mod core_graphics;
    extern mod core_text;

    pub use scaled_font::macos::core_text::font::CTFontRef;
    pub use scaled_font::macos::core_graphics::font::{CGFont, CGFontRef};

    #[cfg(cairo)]
    pub use cairo::cairo_quartz::bindgen::cairo_quartz_font_face_create_for_cgfont;
}

#[cfg(target_os="linux")]
pub mod linux {
    extern mod freetype;

    pub use scaled_font::linux::freetype::freetype::{FT_Face, FT_LOAD_DEFAULT};

    #[cfg(cairo)]
    pub use cairo::cairo_ft::bindgen::cairo_ft_font_face_create_for_ft_face;
}

type SkTypeface = *c_void;

pub struct ScaledFont {
    azure_scaled_font: AzScaledFontRef,
}

impl Drop for ScaledFont {
    fn finalize(&self) {
        unsafe {
            AzReleaseScaledFont(self.azure_scaled_font);
        }
    }
}

impl ScaledFont {
    pub fn get_ref(&self) -> AzScaledFontRef {
        self.azure_scaled_font
    }

    #[cfg(cairo)]
    priv fn create_cairo_font(face: *cairo_font_face_t, size: AzFloat) -> *cairo_scaled_font_t {
        // FIXME: error handling

        let idmatrix: cairo_matrix_t = struct__cairo_matrix {
            xx: 0 as c_double,
            yx: 0 as c_double,
            xy: 0 as c_double,
            yy: 0 as c_double,
            x0: 0 as c_double,
            y0: 0 as c_double
        };
        cairo_matrix_init_identity(ptr::to_unsafe_ptr(&idmatrix));

        let fontmatrix = idmatrix;
        cairo_matrix_scale(ptr::to_unsafe_ptr(&fontmatrix), size as c_double, size as c_double);
        let options = cairo_font_options_create();
        let cfont = cairo_scaled_font_create(face, 
                                             ptr::to_unsafe_ptr(&fontmatrix),
                                             ptr::to_unsafe_ptr(&idmatrix), 
                                             options);
        cairo_font_options_destroy(options);

        return cfont;
    }

    #[cfg(cairo, target_os="linux")]
    fn init_cairo_scaled_font(azure_native_font: &mut struct__AzNativeFont,
                              native_font: FT_Face) {
        azure_native_font.mType = AZ_NATIVE_FONT_CAIRO_FONT_FACE;

        unsafe {
            let cairo_face = cairo_ft_font_face_create_for_ft_face(native_font,
                                                                   FT_LOAD_DEFAULT as c_int);
            if cairo_face.is_null() {
                fail!(~"null cairo face");
            }

            let cairo_font = ScaledFont::create_cairo_font(cairo_face, size);
            cairo_font_face_destroy(cairo_face);

            azure_native_font.mFont = cast::transmute(cairo_font);
        }
    }

    #[cfg(cairo, target_os="macos")]
    fn init_cairo_scaled_font(azure_native_font: &mut struct__AzNativeFont,
                              native_font: &CGFont) {
        azure_native_font.mType = AZ_NATIVE_FONT_CAIRO_FONT_FACE;

        unsafe {
            let face = cairo_quartz_font_face_create_for_cgfont(*native_font.borrow_ref());

            if face == ptr::null() { fail!(~"null cairo face"); }

            let cairo_font = ScaledFont::create_cairo_font(face, size);
            cairo_font_face_destroy(face);

            azure_native_font.mFont = cast::transmute(cairo_font);
        }
    }

    #[cfg(not(cairo))]
    fn init_cairo_scaled_font(_: &mut struct__AzNativeFont) {}

    #[cfg(target_os="linux")]
    pub fn new(backend: BackendType, native_font: FT_Face, size: AzFloat)
        -> ScaledFont {
        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null()
        };
        
        match backend {
            SkiaBackend => {
                unsafe {
                    let options = AzCreateFontOptions((*native_font).family_name, AZ_FONT_STYLE_NORMAL);
                    azure_native_font.mType = AZ_NATIVE_FONT_SKIA_FONT_FACE;
                    azure_native_font.mFont = cast::transmute(options);
                    AzDestroyFontOptions(options)
                }
            }
            CairoBackend => {
                ScaledFont::init_cairo_scaled_font(&mut azure_native_font);
            },
            _ => { fail!(~"don't know how to make a scaled font for this backend"); }
        }

        let azure_native_font_ptr = ptr::to_unsafe_ptr(&azure_native_font);
        unsafe {
            let azure_scaled_font = AzCreateScaledFontForNativeFont(azure_native_font_ptr, size);
            ScaledFont { azure_scaled_font: azure_scaled_font }
        }
    }

    /// Mac-specific function to create a font for the given backend.
    #[cfg(target_os="macos")]
    pub fn new(backend: BackendType, native_font: &CGFont, size: AzFloat) -> ScaledFont {
        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null()
        };

        match backend {
            CoreGraphicsBackend | CoreGraphicsAcceleratedBackend | SkiaBackend => {
                azure_native_font.mType = AZ_NATIVE_FONT_MAC_FONT_FACE;
                unsafe {
                    azure_native_font.mFont = cast::transmute(*native_font.borrow_ref());
                }
            }
            CairoBackend => {
                ScaledFont::init_cairo_scaled_font(&mut azure_native_font);
            }
            NoBackend | Direct2DBackend | RecordingBackend => {
                fail!(~"don't know how to make a scaled font for this backend");
            }
        }

        unsafe {
            let azure_scaled_font = AzCreateScaledFontForNativeFont(&azure_native_font, size);
            ScaledFont {
                azure_scaled_font: azure_scaled_font
            }
        }
    }
}

// FIXME: Move this stuff to a rust-skia?
// FIXME: Demangle the names!!!
#[cfg(target_os="macos")]
#[link_args="-lskia"]
extern {
    pub fn _Z26SkCreateTypefaceFromCTFontPK8__CTFont(font: CTFontRef) -> *SkTypeface;
}


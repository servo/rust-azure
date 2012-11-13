extern mod cairo;

use azure_hl::{BackendType, CairoBackend, CoreGraphicsAcceleratedBackend, CoreGraphicsBackend};
use azure_hl::{Direct2DBackend, NoBackend, RecordingBackend, SkiaBackend};
use bindgen::{AzCreateScaledFontForNativeFont, AzReleaseScaledFont};
use cairo::{cairo_font_face_t, cairo_matrix_t, cairo_scaled_font_t};
use cairo::bindgen::{cairo_font_face_destroy, cairo_font_options_create};
use cairo::bindgen::{cairo_font_options_destroy, cairo_matrix_init_identity, cairo_matrix_scale};
use cairo::bindgen::{cairo_scaled_font_create};

#[cfg(target_os="macos")]
priv mod macos {
    extern mod core_graphics;
    extern mod core_text;
    use core_graphics::font::CGFontRef;
    use core_text::font::{CTFont, CTFontRef};
    use cairo::cairo_quartz::bindgen::cairo_quartz_font_face_create_for_cgfont;

    type NativeFont = CTFont;
}
#[cfg(target_os="macos")]
priv use macos::*;

type SkTypeface = *c_void;

pub struct ScaledFont {
    azure_scaled_font: AzScaledFontRef,

    drop {
        AzReleaseScaledFont(self.azure_scaled_font);
    }
}

impl ScaledFont {
    static priv fn create_cairo_font(face: *cairo_font_face_t, size: AzFloat)
                                  -> *cairo_scaled_font_t {
        // FIXME: error handling

        let idmatrix: cairo_matrix_t = {
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

    /// Mac-specific function to create a font for the given backend.
    #[cfg(target_os="macos")]
    static pub fn new(backend: BackendType, native_font: &const CTFont, size: AzFloat)
                   -> ScaledFont {
        let mut azure_native_font = {
            mType: 0,
            mFont: ptr::null()
        };

        match backend {
            CoreGraphicsBackend | CoreGraphicsAcceleratedBackend | SkiaBackend => {
                azure_native_font.mType = AZ_NATIVE_FONT_MAC_FONT_FACE;

                unsafe {
                    azure_native_font.mFont = cast::transmute(native_font.copy_to_CGFont());
                }
            }
            CairoBackend => {
                azure_native_font.mType = AZ_NATIVE_FONT_CAIRO_FONT_FACE;

                // TODO(Issue #184): may want to just reuse CGFont stored in servo's QuartzFontHandle.
                // This would require creating some trait like CGFontProvider with .get_CGFont().
                let cg_font = native_font.copy_to_CGFont();
                let face = cairo_quartz_font_face_create_for_cgfont(cg_font);
                if face == ptr::null() { fail; }

                let cairo_font = ScaledFont::create_cairo_font(face, size);
                cairo_font_face_destroy(face);

                unsafe {
                    azure_native_font.mFont = cast::transmute(cairo_font);
                }
            }
            NoBackend | Direct2DBackend | RecordingBackend => {
                fail ~"don't know how to make a scaled font for this backend";
            }
        }

        let azure_native_font_ptr = ptr::to_unsafe_ptr(&azure_native_font);
        let azure_scaled_font = AzCreateScaledFontForNativeFont(azure_native_font_ptr, size);
        ScaledFont { azure_scaled_font: move azure_scaled_font }
    }
}

// FIXME: Move this stuff to a rust-skia?
// FIXME: Demangle the names!!!
#[link_args="-lskia"]
extern {
    #[cfg(target_os="macos")]
    pub fn _Z26SkCreateTypefaceFromCTFontPK8__CTFont(font: CTFontRef) -> *SkTypeface;
}


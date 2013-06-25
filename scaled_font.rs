// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

use azure::{AzScaledFontRef, AzFloat};
use azure::{struct__AzNativeFont};

use azure_hl::{BackendType,SkiaBackend};
use azure::{AzCreateScaledFontForNativeFont, AzReleaseScaledFont};

use std::cast;
use std::libc::c_void;
use std::ptr;

#[cfg(target_os="macos")]
priv use scaled_font::macos::*;

#[cfg(target_os="linux")]
priv use scaled_font::linux::*;

#[cfg(target_os="macos")]
pub mod macos {
    extern mod core_graphics;
    extern mod core_text;

    pub use scaled_font::macos::core_text::font::CTFontRef;
    pub use scaled_font::macos::core_graphics::font::{CGFont, CGFontRef};
}

#[cfg(target_os="linux")]
pub mod linux {
    extern mod freetype;

    pub use scaled_font::linux::freetype::freetype::{FT_Face, FT_LOAD_DEFAULT};
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

    #[cfg(target_os="linux")]
    pub fn new(backend: BackendType, native_font: FT_Face, size: AzFloat)
        -> ScaledFont {
        use azure::AZ_NATIVE_FONT_SKIA_FONT_FACE;
        use azure::{AzCreateFontOptions, AzDestroyFontOptions};

        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null()
        };
        
        match backend {
            SkiaBackend => {
                unsafe {
                    // NOTE: azure style flags and freetype style flags are the same in the lowest 2 bits
                    let style = ((*native_font).style_flags & 3) as u32;
                    let options = AzCreateFontOptions((*native_font).family_name, style);
                    azure_native_font.mType = AZ_NATIVE_FONT_SKIA_FONT_FACE;
                    azure_native_font.mFont = cast::transmute(options);
                    let azure_native_font_ptr = ptr::to_unsafe_ptr(&azure_native_font);
                    let azure_scaled_font = AzCreateScaledFontForNativeFont(azure_native_font_ptr, size);
                    AzDestroyFontOptions(options);
                    ScaledFont { azure_scaled_font: azure_scaled_font }
                }
            }
            _ => { fail!(~"don't know how to make a scaled font for this backend"); }
        }
    }

    /// Mac-specific function to create a font for the given backend.
    #[cfg(target_os="macos")]
    pub fn new(backend: BackendType, native_font: &CGFont, size: AzFloat) -> ScaledFont {
        use azure::AZ_NATIVE_FONT_MAC_FONT_FACE;
        use azure_hl::{CoreGraphicsBackend,CoreGraphicsAcceleratedBackend};

        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null()
        };

        match backend {
            CoreGraphicsBackend | CoreGraphicsAcceleratedBackend | SkiaBackend => {
                azure_native_font.mType = AZ_NATIVE_FONT_MAC_FONT_FACE;
                unsafe {
                    azure_native_font.mFont = cast::transmute(*native_font.contents.borrow_ref());
                }
            }
            _ => {
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


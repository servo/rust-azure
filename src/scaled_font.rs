// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#[cfg(target_os = "android")] use scaled_font::android::*;
#[cfg(target_os = "linux")] use scaled_font::linux::*;
#[cfg(target_os = "macos")] use scaled_font::macos::*;
#[cfg(target_os = "windows")] use scaled_font::windows::*;
use azure::{AzCreateScaledFontForNativeFont, AzReleaseScaledFont};
use azure::{AzScaledFontRef, AzFloat, struct__AzNativeFont};
use azure_hl::BackendType;
use libc::c_void;
use std::mem;
use std::ptr;

#[cfg(target_os = "macos")]
pub mod macos {
    pub use core_graphics::font::{CGFont, CGFontRef};
    pub use core_text::font::CTFontRef;
}

#[cfg(target_os = "linux")]
pub mod linux {
    pub use freetype::freetype::{FT_Face, FT_LOAD_DEFAULT};
}

#[cfg(target_os = "android")]
pub mod android {
    pub use freetype::freetype::{FT_Face, FT_LOAD_DEFAULT};
}

#[cfg(target_os = "windows")]
pub mod windows {
    pub use freetype::freetype::{FT_Face, FT_LOAD_DEFAULT};
}

pub type SkTypeface = *mut c_void;

#[cfg(any(target_os = "linux", target_os = "android", target_os = "windows"))]
#[derive(Debug)]
pub enum FontInfo<'a> {
    NativeFont(FT_Face),
    FontData(&'a Vec<u8>),
}

#[derive(Debug)]
pub struct ScaledFont {
    azure_scaled_font: AzScaledFontRef,
}

impl Drop for ScaledFont {
    fn drop(&mut self) {
        unsafe {
            AzReleaseScaledFont(self.azure_scaled_font);
        }
    }
}

impl ScaledFont {
    pub fn get_ref(&self) -> AzScaledFontRef {
        self.azure_scaled_font
    }

    #[cfg(any(target_os = "linux", target_os = "android", target_os = "windows"))]
    pub fn new(backend: BackendType, font_info: FontInfo, size: AzFloat) -> ScaledFont {
        use azure::AZ_NATIVE_FONT_SKIA_FONT_FACE;
        use azure::AzDestroyFontOptions;
        use azure::{AzCreateFontOptionsForName, AzCreateScaledFontForTrueTypeData};

        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null_mut(),
        };

        match backend {
            BackendType::Skia => {
                unsafe {
                    match font_info {
                        FontInfo::NativeFont(native_font) => {
                            // NOTE: azure style flags and freetype style flags are the same in the lowest 2 bits
                            let style = ((*native_font).style_flags & 3) as u32;
                            let options = AzCreateFontOptionsForName(&*(*native_font).family_name,
                                                                     style);
                            azure_native_font.mType = AZ_NATIVE_FONT_SKIA_FONT_FACE;
                            azure_native_font.mFont = mem::transmute(options);
                            let azure_native_font_ptr = &mut azure_native_font;
                            let azure_scaled_font =
                                AzCreateScaledFontForNativeFont(azure_native_font_ptr, size);
                            AzDestroyFontOptions(options);
                            ScaledFont { azure_scaled_font: azure_scaled_font }
                        }
                        FontInfo::FontData(bytes) => {
                            let azure_scaled_font =
                                AzCreateScaledFontForTrueTypeData(bytes.as_ptr(),
                                                                  bytes.len() as u32,
                                                                  0,
                                                                  size,
                                                                  AZ_NATIVE_FONT_SKIA_FONT_FACE);
                            ScaledFont { azure_scaled_font: azure_scaled_font }
                        }
                    }
                }
            }
            _ => {
                panic!("don't know how to make a scaled font for this backend");
            }
        }
    }

    /// Mac-specific function to create a font for the given backend.
    #[cfg(target_os = "macos")]
    pub fn new(backend: BackendType, native_font: &CGFont, size: AzFloat) -> ScaledFont {
        use azure::AZ_NATIVE_FONT_MAC_FONT_FACE;
        use core_foundation::base::TCFType;

        let mut azure_native_font = struct__AzNativeFont {
            mType: 0,
            mFont: ptr::null_mut(),
        };

        match backend {
            BackendType::CoreGraphics |
            BackendType::CoreGraphicsAccelerated |
            BackendType::Skia => {
                azure_native_font.mType = AZ_NATIVE_FONT_MAC_FONT_FACE;
                unsafe {
                    azure_native_font.mFont = mem::transmute(native_font.as_concrete_TypeRef());
                }
            }
            _ => {
                panic!("don't know how to make a scaled font for this backend");
            }
        }

        unsafe {
            let azure_scaled_font = AzCreateScaledFontForNativeFont(&mut azure_native_font, size);
            ScaledFont { azure_scaled_font: azure_scaled_font }
        }
    }
}

// FIXME: Move this stuff to a rust-skia?
// FIXME: Demangle the names!!!
#[cfg(target_os = "macos")]
extern "C" {
    pub fn _Z26SkCreateTypefaceFromCTFontPK8__CTFont(font: CTFontRef) -> *mut SkTypeface;
}

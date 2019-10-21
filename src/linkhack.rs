/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Some crumminess to make sure we link correctly

#[cfg(target_os = "linux")]
#[link(name = "azure", kind = "static")]
#[link(name = "stdc++")]
#[link(name = "freetype")]
#[link(name = "bz2")]
// fontconfig must come before expat for linking to succeed
#[link(name = "fontconfig")]
#[link(name = "expat")]
#[link(name = "X11")]
extern { }

#[cfg(target_os = "android")]
#[link(name = "azure", kind = "static")]
#[link(name = "stdc++")]
#[link(name = "expat")]
#[link(name = "fontconfig")]
#[link(name = "EGL")]
extern { }

#[cfg(target_os = "macos")]
#[link(name = "azure", kind = "static")]
#[link(name = "c++")]
#[link(name = "objc")]
#[link(name = "IOSurface", kind = "framework")]
#[link(name = "OpenGL", kind = "framework")]
#[link(name = "Foundation", kind = "framework")]
#[link(name = "QuartzCore", kind = "framework")]
#[link(name = "ApplicationServices", kind = "framework")]
extern { }

#[cfg(target_os = "freebsd")]
#[link(name = "azure", kind = "static")]
#[link(name = "c++")]
#[link(name = "freetype")]
#[link(name = "bz2")]
#[link(name = "fontconfig")]
#[link(name = "expat")]
#[link(name = "X11")]
extern { }

# Copyright 2013 The Servo Project Developers. See the COPYRIGHT
# file at the top-level directory of this distribution.
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

import argparse, subprocess;

bindgen = "bindgen"

azure = "azure-c.h"
includes = [
    "-I", "include"
    ]
sysincludes = [
    "-isystem", "/usr/lib/x86_64-linux-gnu/gcc/x86_64-linux-gnu/4.5/include",
    "-isystem", "/usr/lib/x86_64-linux-gnu/gcc/x86_64-linux-gnu/4.5/include-fixed",
    "-isystem", "/usr/lib/gcc/x86_64-linux-gnu/4.6/include",
    "-isystem", "/usr/include/cairo",
    "-isystem", "/usr/include/freetype2",
    ]
otherflags = [
    "-DMOZ_GFX",
    "-DUSE_CAIRO",
    "-DNS_ATTR_MALLOC=",
    "-DNS_WARN_UNUSED_RESULT="
    ]

args = [
    bindgen,
    "-l", "azure",
    "-o", "azure.rs",
    "-match", "azure-c",
    azure]
args += includes + sysincludes + otherflags

subprocess.call(args)

cairo = "/usr/include/cairo/cairo.h"
args = [
    bindgen,
    "-l", "cairo",
    "-o", "cairo.rs",
    "-match", "cairo",
    cairo]
args += includes + sysincludes

subprocess.call(args)

cairo_ft = "/usr/include/cairo/cairo-ft.h"
args = [
    bindgen,
    "-l", "cairo",
    "-o", "cairo_ft.rs",
    "-match", "cairo-ft",
    cairo_ft]
args += includes + sysincludes + ["-DCAIRO_HAS_FT_FONT", "-DCAIRO_HAS_FC_FONT"]

subprocess.call(args)

cairo_xlib = "/usr/include/cairo/cairo-xlib.h"
args = [
    bindgen,
    "-l", "cairo",
    "-o", "cairo_xlib.rs",
    "-match", "cairo-xlib",
    cairo_xlib]
args += includes + sysincludes

subprocess.call(args)

ft = "/usr/include/freetype2/freetype/freetype.h"
args = [
    bindgen,
    "-l", "freetype",
    "-o", "freetype.rs",
    "-match", "freetype",
    cairo_ft]
args += includes + sysincludes

subprocess.call(args)

fc = "/usr/include/fontconfig/fontconfig.h"
args = [
    bindgen,
    "-l", "fontconfig",
    "-o", "fontconfig.rs",
    "-match", "fontconfig",
    cairo_ft]
args += includes + sysincludes

subprocess.call(args)

xlib = "/usr/include/X11/Xlib.h"
args = [
    bindgen,
    "-l", "X11",
    "-o", "xlib.rs",
    "-match", "Xlib",
    "-match", "X",
    xlib]
args += includes + sysincludes

subprocess.call(args)
        

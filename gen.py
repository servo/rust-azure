import argparse, subprocess;

bindgen = "bindgen"

azure = "azure-c.h"
includes = [
    "-I", "include"
    ]
sysincludes = [
    "-isystem", "/usr/lib/x86_64-linux-gnu/gcc/x86_64-linux-gnu/4.5/include",
    "-isystem", "/usr/lib/gcc/x86_64-linux-gnu/4.6/include"
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

cairo_xlib = "/usr/include/cairo/cairo-xlib.h"
args = [
    bindgen,
    "-l", "cairo",
    "-o", "cairo_xlib.rs",
    "-match", "cairo-xlib",
    cairo_xlib]
args += includes + sysincludes

subprocess.call(args)

xlib = "/usr/include/X11/Xlib.h"
args = [
    bindgen,
    "-l", "X",
    "-o", "xlib.rs",
    "-match", "Xlib",
    "-match", "X",
    xlib]
args += includes + sysincludes

subprocess.call(args)
        

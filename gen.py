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
    "-match" ,"azure-c",
    azure]
args += includes + sysincludes + otherflags

subprocess.call(args)
        
        

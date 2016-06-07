/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

extern crate cmake;
//use std::process::Command;
use std::env;

fn main() {
    let dst = cmake::Config::new(".").generator("Ninja")
        .define("CMAKE_C_COMPILER", "cl.exe")
        .define("CMAKE_CXX_COMPILER", "cl.exe")
        .define("CMAKE_LINKER", "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\amd64\\link.exe")
        .build();
    println!("cargo:rustc-link-search=native={}/lib", dst.display());

    let target = env::var("TARGET").unwrap();
    if target.contains("windows") {
        println!("cargo:rustc-link-lib=static=azure");
        println!("cargo:rustc-link-lib=uuid");
        if target.contains("gnu") {
            println!("cargo:rustc-link-lib=stdc++");
        }
    }
}

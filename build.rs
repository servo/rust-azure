/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use std::process::Command;
use std::env;


fn main() {
    assert!(Command::new("make")
        .args(&["-R", "-f", "makefile.cargo", &format!("-j{}", env::var("NUM_JOBS").unwrap())])
        .status()
        .unwrap()
        .success());
    println!("cargo:rustc-flags=-L native={}", env::var("OUT_DIR").unwrap());

    let target = env::var("TARGET").unwrap();
    // We need a lot of stuff on Windows and we need to resolve static
    // link order.. so just list things a few extra times.
    if target.contains("windows") {
        println!("cargo:rustc-link-lib=skia");
        println!("cargo:rustc-link-lib=freetype");
        println!("cargo:rustc-link-lib=fontconfig");
    }
}

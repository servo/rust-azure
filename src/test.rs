/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// The azure unit tests require this
// on Linux to link correctly.

#[cfg(target_os = "linux")]
#[link(name = "GL")]
extern { }

#[test]
fn sanity_check() {
    use azure::AzSanityCheck;

    unsafe { AzSanityCheck() };
}

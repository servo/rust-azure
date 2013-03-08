use azure::bindgen::AzSanityCheck;

#[test]
fn sanity_check() {
    unsafe { AzSanityCheck() };
}

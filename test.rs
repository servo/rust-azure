import bindgen::*;

#[link_args = "-L. -lcairo -lazure"]
#[nolink]
native mod m { }

#[test]
fn sanity_check() {
    AzureCSanityCheck();
}

#[test]
fn cairo_it_up() {
    
}
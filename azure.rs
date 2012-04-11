#[link(name = "azure",
       vers = "0.1")];
#[crate_type = "lib"];

use std;

#[link_args = "-L."]
native mod azure {
    fn AzureCSanityCheck();
}

#[test]
fn sanity_check() {
    azure::AzureCSanityCheck();
}

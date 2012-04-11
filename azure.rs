#[crate_type = "lib"];

use std;

#[link_args = "-L."]
native mod azure {
}

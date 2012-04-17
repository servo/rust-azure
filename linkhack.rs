// Some crumminess to make sure we link correctly
#[link_args = "-L. -lcairo -lazure"]
#[nolink]
native mod m { }

// Some crumminess to make sure we link correctly

#[cfg(target_os = "linux")]
#[link_args = "-L. -lcairo -lazure"]
#[nolink]
native mod m { }

#[cfg(target_os = "macos")]
#[link_args = "-L. -lcairo -lazure -lstdc++ -framework ApplicationServices"]
#[nolink]
native mod m { }

// Some crumminess to make sure we link correctly

#[cfg(target_os = "linux")]
#[link_args = "-L. -lazure -lstdc++ -lskia"]
#[nolink]
extern mod m { }

#[cfg(target_os = "macos")]
#[link_args = "-L. -lazure -lstdc++ -framework ApplicationServices \
			   -lskia -framework IOSurface -lobjc -framework OpenGL \
			   -framework Foundation -framework QuartzCore"]
#[nolink]
extern mod m { }

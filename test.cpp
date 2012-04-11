#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <X11/Xlib.h>
//#include "xcb/xcb.h"
#include "cairo/cairo.h"
#include "cairo/cairo-xlib.h"
#include "mozilla/gfx/2D.h"

/*int main() {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);
  const xcb_setup_t *setup = xcb_get_setup(connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
  xcb_screen_t *screen = iter.data;

  xcb_window_t window = xcb_generate_id(connection);
  xcb_create_window(connection,
                    XCB_COPY_FROM_PARENT,
                    window,
                    screen->root,
                    0, 0,
                    150, 150,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    0, NULL);

  xcb_map_window(connection, window);
  xcb_flush(connection);

  pause();
  xcb_disconnect(connection);

  return 0;
}
*/

#define SIZEX 400
#define SIZEY 400

void paint(cairo_surface_t *cs) {
    cairo_t *c;
    c = cairo_create(cs);
    cairo_rectangle(c, 0.0, 0.0, SIZEX, SIZEY);
    cairo_set_source_rgb(c, 0.0, 0.0, 0.5);
    cairo_fill(c);
}

using namespace mozilla;
using namespace mozilla::gfx;

void paint3() { }

void paint2(RefPtr<DrawTarget> drawTarget) {
    Rect rect = Rect(50, 50, 200, 200);
    ColorPattern pattern = ColorPattern(Color(0.0, 0.5, 0.0));
    drawTarget->FillRect(rect, pattern);
}

int main() {
    Display *dpy;
    Window rootwin;
    Window win;
    XEvent e;
    int scr;
    cairo_surface_t *cs;

    dpy = XOpenDisplay(NULL);
    assert(dpy);

    scr = DefaultScreen(dpy);
    rootwin = RootWindow(dpy, scr);
    win = XCreateSimpleWindow(dpy, rootwin, 1, 1, SIZEX, SIZEY, 0,
			      BlackPixel(dpy, scr), BlackPixel(dpy, scr));

    XStoreName(dpy, win, "test");
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);
    XMapWindow(dpy, win);

    cs = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), SIZEX, SIZEY);

    RefPtr<DrawTarget> dt = Factory::CreateDrawTargetForCairoSurface(cs);

    while(1) {
	XNextEvent(dpy, &e);
	if (e.type==Expose && e.xexpose.count<1) {
	    //paint(cs);
	    paint2(dt);
	} else if (e.type==ButtonPress) break;
    }

    cairo_surface_destroy(cs);
    XCloseDisplay(dpy);

    return 0;
}

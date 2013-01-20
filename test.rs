extern mod xlib;
extern mod cairo;

use azure::{AzDrawTargetRef, AzFloat};
use test::xlib::xlib::XExposeEvent;
use test::xlib::xlib::bindgen::{XOpenDisplay, XDefaultScreen, XRootWindow};
use test::xlib::xlib::bindgen::{XBlackPixel, XCreateSimpleWindow, XStoreName};
use test::xlib::xlib::bindgen::{XSelectInput, XCloseDisplay, XNextEvent, XDefaultVisual};
use test::xlib::xlib::bindgen::{XMapWindow};
use azure::bindgen::{AzCreateColorPattern, AzDrawTargetFillRect, AzReleaseColorPattern};
use azure::bindgen::{AzSanityCheck, AzCreateDrawTargetForCairoSurface, AzReleaseDrawTarget};
use azure::bindgen::{AzDrawTargetGetSize};
use test::cairo::cairo::{CAIRO_FORMAT_RGB24};
use test::cairo::cairo::bindgen::{cairo_image_surface_create, cairo_surface_destroy};
use test::cairo::cairo_xlib::bindgen::{cairo_xlib_surface_create};

use core::libc::{c_uint, c_long, c_int};

#[test]
fn sanity_check() {
    AzSanityCheck();
}

const SIZEX: c_uint = 400 as c_uint;
const SIZEY: c_uint = 400 as c_uint;

const ExposureMask: c_long = (1 << 15) as c_long;
const ButtonPressMask: c_long = (1 << 2) as c_long;

const Expose: c_int = 12 as c_int;
const ButtonPress: c_int = 4 as c_int;

type XEventStub = {
    type_: c_int,
    padding: (
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int,
        int, int, int, int, int, int, int, int
    )
};

fn xexpose(event: *XEventStub) -> *XExposeEvent unsafe {
    cast::reinterpret_cast(&ptr::addr_of(&(*event).padding))
}

#[test]
#[ignore]
fn cairo_it_up() {
    let dpy = XOpenDisplay(ptr::null());
    assert(ptr::is_not_null(dpy));
    let scr = XDefaultScreen(dpy);
    let rootwin = XRootWindow(dpy, scr);
    let win = XCreateSimpleWindow(
        dpy, rootwin, 1 as c_int, 1 as c_int, SIZEX, SIZEY, 0 as c_uint,
        XBlackPixel(dpy, scr), XBlackPixel(dpy, scr));
    str::as_c_str(~"test", |cstr| XStoreName(dpy, win, cstr));
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);
    XMapWindow(dpy, win);

    let cs = cairo_xlib_surface_create(
        dpy, win, XDefaultVisual(dpy, 0 as c_int),
        SIZEX as c_int, SIZEY as c_int);

    let e = {
        type_: 0 as c_int,
        padding: (
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        )
    };
    let ep = ptr::addr_of(&e);

    let dt = AzCreateDrawTargetForCairoSurface(cs, { width: SIZEX, height: SIZEY });

    loop {
        unsafe {
            XNextEvent(dpy, cast::reinterpret_cast(&ep));
            log(error, *ep);
            log(error, *xexpose(ep));
            if e.type_ == Expose && (*xexpose(ep)).count < 1 as c_int {
                paint(dt);
            } else if e.type_ == ButtonPress {
                break;
            } else {
                paint(dt);
            }
        }
    }

    AzReleaseDrawTarget(dt);
    cairo_surface_destroy(cs);
    XCloseDisplay(dpy);
}

fn paint(dt: AzDrawTargetRef) {
    log(error, "painting");
    let rect = {
        x: 200f as AzFloat,
        y: 200f as AzFloat,
        width: 100f as AzFloat,
        height: 100f as AzFloat
    };
    let color = {
        r: 0f as AzFloat,
        g: 1f as AzFloat,
        b: 0f as AzFloat,
        a: 1f as AzFloat
    };
    let pattern = AzCreateColorPattern(ptr::addr_of(&color));
    AzDrawTargetFillRect(
        dt,
        ptr::addr_of(&rect),
        unsafe { cast::reinterpret_cast(&pattern) });
    AzReleaseColorPattern(pattern);
}

#[test]
#[ignore(reason="busted on linux 32 bit")]
fn test_draw_target_get_size() {
    let surface = cairo_image_surface_create(
        CAIRO_FORMAT_RGB24, 100 as c_int, 200 as c_int);
    assert surface.is_not_null();
    let dt = AzCreateDrawTargetForCairoSurface(surface);
    assert dt.is_not_null();
    let size = AzDrawTargetGetSize(dt);
    assert size.width == 100 as int32_t;
    assert size.height == 200 as int32_t;
    AzReleaseDrawTarget(dt);
    cairo_surface_destroy(surface);
}

#[test]
#[ignore]
fn fonts() {
    use cairo::*;
    use cairo::bindgen::*;

    let dpy = XOpenDisplay(ptr::null());
    assert(ptr::is_not_null(dpy));
    let scr = XDefaultScreen(dpy);
    let rootwin = XRootWindow(dpy, scr);
    let win = XCreateSimpleWindow(
        dpy, rootwin, 1 as c_int, 1 as c_int, SIZEX, SIZEY, 0 as c_uint,
        XBlackPixel(dpy, scr), XBlackPixel(dpy, scr));
    str::as_c_str(~"test", |cstr| XStoreName(dpy, win, cstr));
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);
    XMapWindow(dpy, win);

    let cs = cairo_xlib_surface_create(
        dpy, win, XDefaultVisual(dpy, 0 as c_int),
        SIZEX as c_int, SIZEY as c_int);

    let e = {
        type_: 0 as c_int,
        padding: (
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        )
    };
    let ep = ptr::addr_of(&e);

    loop {
        unsafe {
            XNextEvent(dpy, cast::reinterpret_cast(&ep));
            log(error, *ep);
            log(error, *xexpose(ep));
            if e.type_ == Expose && (*xexpose(ep)).count < 1 as c_int {
                paint(cs);
            } else if e.type_ == ButtonPress {
                break;
            } else {
                paint(cs);
            }
        }
    }

    cairo_surface_destroy(cs);
    XCloseDisplay(dpy);

    fn paint(surf: *cairo_surface_t) {
        use libc::c_double;
        use cairo::*;
        use cairo::bindgen::*;

        let cr: *cairo_t = cairo_create(surf);
        assert cr.is_not_null();

        cairo_set_source_rgb(cr, 0.5 as c_double, 0.1 as c_double, 0.1 as c_double);
        cairo_paint(cr);

        let te: cairo_text_extents_t = {
            x_bearing: 0 as c_double,
            y_bearing: 0 as c_double,
            width: 0 as c_double,
            height: 0 as c_double,
            x_advance: 0 as c_double,
            y_advance: 0 as c_double
        };
        cairo_set_source_rgb(cr, 0.9 as c_double, 0.5 as c_double, 0.5 as c_double);
        str::as_c_str(~"Georgia", |fontname| {
            cairo_select_font_face(cr, fontname, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        });
        cairo_set_font_size(cr, 20.2 as c_double);
        str::as_c_str(~"a", |text| {
            cairo_text_extents(cr, text, ptr::addr_of(&te));
            cairo_move_to(
                cr,
                (100.0 + 0.5) as c_double -
                te.width / (2.0 as c_double) - te.x_bearing,
                (100.0 + 0.5) as c_double -
                te.height / (2.0 as c_double) - te.y_bearing);
            cairo_show_text(cr, text);
        });

        cairo_destroy(cr);
    }
}


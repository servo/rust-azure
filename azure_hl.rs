// High-level bindings to Azure.

import bindgen::{AzCreateColorPattern, AzCreateDrawTargetForCairoSurface};
import bindgen::{AzDrawTargetClearRect, AzDrawTargetFillRect, AzDrawTargetFlush};
import bindgen::{AzDrawTargetStrokeRect, AzReleaseColorPattern};
import cairo_hl::ImageSurface;
import geom::rect::Rect;
import ptr::{addr_of, null};

impl AzureExtensions for Rect<AzFloat> {
    fn as_azure_rect() -> AzRect {
        {
            x: self.origin.x,
            y: self.origin.y,
            width: self.size.width,
            height: self.size.height
        }
    }
}

class Color {
    let r: AzFloat;
    let g: AzFloat;
    let b: AzFloat;
    let a: AzFloat;

    new(r: AzFloat, g: AzFloat, b: AzFloat, a: AzFloat) {
        self.r = r;
        self.g = g;
        self.b = b;
        self.a = a;
    }

    fn as_azure_color() -> AzColor {
        { r: self.r, g: self.g, b: self.b, a: self.a }
    }
}

// FIXME: Should have a class hierarchy here starting with Pattern.
class ColorPattern {
    let azure_color_pattern: AzColorPatternRef;

    new(+color: Color) {
        self.azure_color_pattern = AzCreateColorPattern(addr_of(color.as_azure_color()));
    }

    drop {
        AzReleaseColorPattern(self.azure_color_pattern);
    }
}

class StrokeOptions {
    let line_width: AzFloat;
    let miter_limit: AzFloat;
    let fields: uint16_t;

    new(line_width: AzFloat, miter_limit: AzFloat, fields: uint16_t) {
        self.line_width = line_width;
        self.miter_limit = miter_limit;
        self.fields = fields;
    }

    fn as_azure_stroke_options() -> AzStrokeOptions {
        {
            mLineWidth: self.line_width,
            mMiterLimit: self.miter_limit,
            mDashPattern: null(),
            mDashLength: 0,
            mDashOffset: 0.0f as AzFloat,
            fields: 0
        }
    }
}

class DrawOptions {
    let alpha: AzFloat;
    let fields: uint16_t;

    new(alpha: AzFloat, fields: uint16_t) {
        self.alpha = alpha;
        self.fields = fields;
    }

    fn as_azure_draw_options() -> AzDrawOptions {
        {
            mAlpha: self.alpha,
            fields: self.fields
        }
    }
}

class DrawTarget {
    let azure_draw_target: AzDrawTargetRef;

    new(&&cairo_surface: ImageSurface) {
        self.azure_draw_target = AzCreateDrawTargetForCairoSurface(cairo_surface.cairo_surface);
    }

    drop {
        AzReleaseDrawTarget(self.azure_draw_target);
    }

    fn flush() {
        AzDrawTargetFlush(self.azure_draw_target);
    }

    fn clear_rect(&&rect: Rect<AzFloat>) {
        AzDrawTargetClearRect(self.azure_draw_target, addr_of(rect.as_azure_rect()));
    }

    fn fill_rect(&&rect: Rect<AzFloat>, &&pattern: ColorPattern) {
        AzDrawTargetFillRect(self.azure_draw_target,
                             addr_of(rect.as_azure_rect()),
                             pattern.azure_color_pattern);
    }

    fn stroke_rect(&&rect: Rect<AzFloat>, &&pattern: ColorPattern, &&stroke_options: StrokeOptions,
                   &&draw_options: DrawOptions) {
        AzDrawTargetStrokeRect(self.azure_draw_target,
                               addr_of(rect.as_azure_rect()),
                               pattern.azure_color_pattern,
                               addr_of(stroke_options.as_azure_stroke_options()),
                               addr_of(draw_options.as_azure_draw_options()));
    }
}


#include "../internal.h"
#include <stdarg.h>
#include <stdio.h>

static GE_Cell make_blank_cell(GE_Color color) {
    GE_Cell cell = { ' ', color, {0, 0, 0}, {0, 0, 0}, 0, 0 };
    return cell;
}

static int canvas_contains(const GE_Canvas *canvas, int x, int y) {
    return canvas && canvas->cells && x >= 0 && y >= 0 &&
           x < canvas->width && y < canvas->height;
}

int ge_canvas_width(const GE_Canvas *canvas) {
    return canvas ? canvas->width : 0;
}

int ge_canvas_height(const GE_Canvas *canvas) {
    return canvas ? canvas->height : 0;
}

void ge_canvas_clear(GE_Canvas *canvas, GE_Color color) {
    if (!canvas || !canvas->cells) return;
    GE_Cell blank = make_blank_cell(color);
    int cell_count = canvas->width * canvas->height;
    for (int i = 0; i < cell_count; ++i) canvas->cells[i] = blank;
}

void ge_canvas_put(GE_Canvas *canvas, int x, int y, char glyph, GE_Color color) {
    if (!canvas_contains(canvas, x, y)) return;
    canvas->cells[y * canvas->width + x] =
        (GE_Cell){ glyph, color, {0, 0, 0}, {0, 0, 0}, 0, 0 };
}

void ge_canvas_put_pixel(GE_Canvas *canvas, int x, int y, GE_ColorRGB color) {
    if (!canvas_contains(canvas, x, y)) return;
    GE_Cell *cell = &canvas->cells[y * canvas->width + x];
    cell->glyph = ' ';
    cell->color = GE_COLOR_DEFAULT;
    cell->background = color;
    cell->use_background = 1;
    cell->use_foreground = 0;
}

void ge_canvas_put_rgb(GE_Canvas *canvas, int x, int y, char glyph,
                       GE_ColorRGB foreground, GE_ColorRGB background) {
    if (!canvas_contains(canvas, x, y)) return;
    canvas->cells[y * canvas->width + x] =
        (GE_Cell){ glyph, GE_COLOR_DEFAULT, foreground, background, 1, 1 };
}

void ge_canvas_text(GE_Canvas *canvas, int x, int y, const char *text, GE_Color color) {
    if (!canvas || !text || y < 0 || y >= canvas->height) return;
    for (; *text && x < canvas->width; ++text, ++x) {
        if (x >= 0) ge_canvas_put(canvas, x, y, *text, color);
    }
}

void ge_canvas_textf(GE_Canvas *canvas, int x, int y, GE_Color color,
                     const char *format, ...) {
    char buffer[512];
    va_list arguments;
    va_start(arguments, format);
    vsnprintf(buffer, sizeof(buffer), format, arguments);
    va_end(arguments);
    ge_canvas_text(canvas, x, y, buffer, color);
}

void ge_canvas_present(GE_Canvas *canvas) {
    if (canvas) ge_platform_present(canvas);
}

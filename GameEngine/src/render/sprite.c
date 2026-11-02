#include "../internal.h"
#include <stdlib.h>

static GE_Cell transparent_cell(void) {
    GE_Cell cell = { ' ', GE_COLOR_TRANSPARENT, {0, 0, 0}, {0, 0, 0}, 0, 0 };
    return cell;
}

void ge_sprite_init(GE_Sprite *sprite, int width, int height) {
    if (!sprite || width <= 0 || height <= 0) return;
    sprite->width = width;
    sprite->height = height;
    sprite->cells = (GE_Cell *)malloc((size_t)width * (size_t)height * sizeof(*sprite->cells));
    if (!sprite->cells) {
        sprite->width = 0;
        sprite->height = 0;
        return;
    }
    for (int i = 0; i < width * height; ++i) sprite->cells[i] = transparent_cell();
}

void ge_sprite_destroy(GE_Sprite *sprite) {
    if (!sprite) return;
    free(sprite->cells);
    sprite->cells = NULL;
    sprite->width = 0;
    sprite->height = 0;
}

void ge_sprite_set(GE_Sprite *sprite, int x, int y, char glyph, GE_Color color) {
    if (!sprite || !sprite->cells || x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return;
    sprite->cells[y * sprite->width + x] = (GE_Cell){ glyph, color, {0, 0, 0}, {0, 0, 0}, 0, 0 };
}

void ge_sprite_set_pixel(GE_Sprite *sprite, int x, int y, GE_ColorRGB color) {
    if (!sprite || !sprite->cells || x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return;
    sprite->cells[y * sprite->width + x] = (GE_Cell){ ' ', GE_COLOR_DEFAULT, {0, 0, 0}, color, 1, 0 };
}

void ge_sprite_set_rgb(GE_Sprite *sprite, int x, int y, char glyph,
                       GE_ColorRGB foreground, GE_ColorRGB background) {
    if (!sprite || !sprite->cells || x < 0 || y < 0 || x >= sprite->width || y >= sprite->height) return;
    sprite->cells[y * sprite->width + x] =
        (GE_Cell){ glyph, GE_COLOR_DEFAULT, foreground, background, 1, 1 };
}

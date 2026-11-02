#ifndef PL2_GAME_ENGINE_RENDER_H
#define PL2_GAME_ENGINE_RENDER_H
#include "component.h"

#ifdef __cplusplus
extern "C" {
#endif

GE_SpriteRenderer *ge_sprite_renderer_add(GE_GameObject *);
void ge_sprite_renderer_set_sprite(GE_SpriteRenderer *, GE_Sprite *);
GE_Sprite *ge_sprite_renderer_sprite(const GE_SpriteRenderer *);
void ge_engine_draw_objects(GE_Engine *engine);

int ge_canvas_width(const GE_Canvas *canvas);
int ge_canvas_height(const GE_Canvas *canvas);
void ge_canvas_clear(GE_Canvas *, GE_Color);
void ge_canvas_put(GE_Canvas *, int x, int y, char glyph, GE_Color);
void ge_canvas_put_pixel(GE_Canvas *, int x, int y, GE_ColorRGB);
void ge_canvas_put_rgb(GE_Canvas *, int x, int y, char glyph,
                       GE_ColorRGB foreground, GE_ColorRGB background);
void ge_canvas_text(GE_Canvas *, int x, int y, const char *, GE_Color);
void ge_canvas_textf(GE_Canvas *, int x, int y, GE_Color, const char *format, ...);
void ge_canvas_present(GE_Canvas *);

void ge_sprite_init(GE_Sprite *, int width, int height);
void ge_sprite_destroy(GE_Sprite *);
void ge_sprite_set(GE_Sprite *, int x, int y, char glyph, GE_Color);
void ge_sprite_set_pixel(GE_Sprite *, int x, int y, GE_ColorRGB);
void ge_sprite_set_rgb(GE_Sprite *, int x, int y, char glyph,
                       GE_ColorRGB foreground, GE_ColorRGB background);
void ge_camera_set(GE_Camera *, float x, float y);

#ifdef __cplusplus
}
#endif
#endif

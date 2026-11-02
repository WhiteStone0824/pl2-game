#include "../internal.h"
#include <math.h>

static void draw_sprite_cell(GE_Canvas *canvas, GE_Cell cell, int x, int y) {
    if (cell.color == GE_COLOR_TRANSPARENT && !cell.use_background) return;
    if (cell.use_background) {
        if (cell.use_foreground)
            ge_canvas_put_rgb(canvas, x, y, cell.glyph, cell.foreground, cell.background);
        else
            ge_canvas_put_pixel(canvas, x, y, cell.background);
    } else {
        ge_canvas_put(canvas, x, y, cell.glyph, cell.color);
    }
}

static void draw_renderer(GE_Canvas *canvas, const GE_SpriteRenderer *renderer,
                          const GE_GameObject *object, const GE_Camera *camera) {
    const GE_Sprite *sprite = renderer->sprite;
    if (!renderer->visible || !sprite || !sprite->cells) return;

    float scale_x = object->transform.scale_x;
    float scale_y = object->transform.scale_y;
    if (scale_x == 0.0f || scale_y == 0.0f) return;

    int width = (int)(sprite->width * fabsf(scale_x) + 0.5f);
    int height = (int)(sprite->height * fabsf(scale_y) + 0.5f);
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    float radians = object->transform.rotation * 0.01745329251994329577f;
    float cosine = cosf(radians);
    float sine = sinf(radians);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int source_x = (int)(x / fabsf(scale_x));
            int source_y = (int)(y / fabsf(scale_y));
            if (scale_x < 0) source_x = sprite->width - 1 - source_x;
            if (scale_y < 0) source_y = sprite->height - 1 - source_y;

            float local_x = (scale_x < 0 ? -1.0f : 1.0f) * (x + 0.5f);
            float local_y = (scale_y < 0 ? -1.0f : 1.0f) * (y + 0.5f);
            float camera_x = camera ? camera->x : 0.0f;
            float camera_y = camera ? camera->y : 0.0f;
            int screen_x = (int)floorf(object->transform.x + local_x * cosine -
                                       local_y * sine - camera_x);
            int screen_y = (int)floorf(object->transform.y + local_x * sine +
                                       local_y * cosine - camera_y);
            draw_sprite_cell(canvas, sprite->cells[source_y * sprite->width + source_x],
                             screen_x, screen_y);
        }
    }
}

void ge_engine_draw_objects(GE_Engine *engine) {
    if (!engine) return;
    for (size_t i = 0; i < engine->object_count; ++i) {
        GE_GameObject *object = engine->objects[i];
        if (!ge_game_object_is_active(object)) continue;

        for (size_t j = 0; j < object->component_count; ++j) {
            GE_Component *component = object->components[j];
            if (!component->enabled || component->type_id != GE_COMPONENT_SPRITE_RENDERER) continue;
            draw_renderer(&engine->canvas, (GE_SpriteRenderer *)component,
                          object, &engine->camera);
        }
    }
}

GE_SpriteRenderer *ge_sprite_renderer_add(GE_GameObject *object) {
    const GE_ComponentDescriptor descriptor = {
        .size = sizeof(GE_SpriteRenderer),
        .type_id = GE_COMPONENT_SPRITE_RENDERER
    };
    GE_SpriteRenderer *renderer =
        (GE_SpriteRenderer *)ge_game_object_add_component(object, &descriptor);
    if (renderer) renderer->visible = 1;
    return renderer;
}

void ge_sprite_renderer_set_sprite(GE_SpriteRenderer *renderer, GE_Sprite *sprite) {
    if (renderer) renderer->sprite = sprite;
}

GE_Sprite *ge_sprite_renderer_sprite(const GE_SpriteRenderer *renderer) {
    return renderer ? renderer->sprite : NULL;
}

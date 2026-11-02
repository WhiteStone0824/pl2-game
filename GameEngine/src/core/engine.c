#include "../internal.h"
#include <stdlib.h>

GE_Engine *ge_engine_create(const GE_EngineConfig *config) {
    GE_Engine *engine = (GE_Engine *)calloc(1, sizeof(*engine));
    if (!engine) return NULL;

    engine->config.title = config && config->title ? config->title : "CUI Game";
    engine->config.target_fps = config && config->target_fps > 0 ? config->target_fps : 30;
    engine->config.cell_width = config && config->cell_width > 0 ? config->cell_width : 2;
    engine->config.hide_cursor = config ? config->hide_cursor : 1;
    engine->config.enable_color = config ? config->enable_color : 1;
    if (!ge_platform_init(engine)) {
        free(engine);
        return NULL;
    }

    int terminal_width = 80;
    int terminal_height = 24;
    ge_platform_size(&terminal_width, &terminal_height);
    engine->canvas.width = terminal_width / engine->config.cell_width;
    engine->canvas.height = terminal_height;
    engine->canvas.cell_width = engine->config.cell_width;
    engine->canvas.enable_color = engine->config.enable_color;

    if (engine->canvas.width < 1 || engine->canvas.height < 3) {
        ge_platform_shutdown(engine);
        free(engine);
        return NULL;
    }

    size_t cell_count = (size_t)engine->canvas.width * (size_t)engine->canvas.height;
    engine->canvas.cells = (GE_Cell *)calloc(cell_count, sizeof(GE_Cell));
    engine->canvas.previous = (GE_Cell *)calloc(cell_count, sizeof(GE_Cell));
    if (!engine->canvas.cells || !engine->canvas.previous) {
        ge_engine_destroy(engine);
        return NULL;
    }

    engine->running = 1;
    return engine;
}

void ge_engine_destroy(GE_Engine *engine) {
    if (!engine) return;
    for (size_t i = 0; i < engine->object_count; ++i)
        ge_game_object_free(engine->objects[i]);
    free(engine->objects);
    free(engine->canvas.cells);
    free(engine->canvas.previous);
    ge_platform_shutdown(engine);
    free(engine);
}

void ge_engine_set_callbacks(GE_Engine *engine, GE_InitCallback init,
                             GE_UpdateCallback update, GE_RenderCallback render,
                             void *user_data) {
    if (!engine) return;
    engine->init = init;
    engine->update = update;
    engine->render = render;
    engine->user_data = user_data;
}

GE_Canvas *ge_engine_canvas(GE_Engine *engine) {
    return engine ? &engine->canvas : NULL;
}

GE_Input *ge_engine_input(GE_Engine *engine) {
    return engine ? &engine->input : NULL;
}

GE_Camera *ge_engine_camera(GE_Engine *engine) {
    return engine ? &engine->camera : NULL;
}

void ge_engine_quit(GE_Engine *engine) {
    if (engine) engine->running = 0;
}

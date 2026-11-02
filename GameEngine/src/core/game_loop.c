#include "../internal.h"
#include <string.h>

static void remove_destroyed_objects(GE_Engine *engine) {
    size_t active_count = 0;
    for (size_t i = 0; i < engine->object_count; ++i) {
        GE_GameObject *object = engine->objects[i];
        if (object->pending_destroy) {
            ge_game_object_free(object);
        } else {
            engine->objects[active_count++] = object;
        }
    }
    engine->object_count = active_count;
}

static void update_components(GE_Engine *engine, size_t object_count, double delta_seconds) {
    if (object_count > engine->object_count) object_count = engine->object_count;
    for (size_t i = 0; i < object_count; ++i)
        ge_game_object_update_components(engine->objects[i], delta_seconds);
}

int ge_engine_run(GE_Engine *engine) {
    if (!engine) return 1;
    if (engine->init) engine->init(engine, engine->user_data);

    double previous_time = ge_platform_time();
    int frame_duration_ms = 1000 / engine->config.target_fps;
    while (engine->running) {
        double frame_start = ge_platform_time();
        memset(engine->input.pressed, 0, sizeof(engine->input.pressed));
        ge_platform_poll(&engine->input);

        double current_time = ge_platform_time();
        double delta_seconds = current_time - previous_time;
        if (engine->update) engine->update(engine, delta_seconds, engine->user_data);

        size_t object_count = engine->object_count;
        update_components(engine, object_count, delta_seconds);
        memset(engine->input.down, 0, sizeof(engine->input.down));
        previous_time = current_time;

        if (engine->render) engine->render(engine, engine->user_data);
        ge_engine_draw_objects(engine);
        ge_canvas_present(&engine->canvas);
        remove_destroyed_objects(engine);

        int elapsed_ms = (int)((ge_platform_time() - frame_start) * 1000.0);
        if (elapsed_ms < frame_duration_ms)
            ge_platform_sleep(frame_duration_ms - elapsed_ms);
    }
    return 0;
}

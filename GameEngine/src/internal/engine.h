#ifndef PL2_GE_ENGINE_INTERNAL_H
#define PL2_GE_ENGINE_INTERNAL_H
#include "../../include/game_engine.h"

struct GE_Canvas {
    int width;
    int height;
    int cell_width;
    int enable_color;
    GE_Cell *cells;
    GE_Cell *previous;
    int previous_valid;
};

struct GE_Input {
    unsigned char down[512];
    unsigned char pressed[512];
    unsigned char previous[512];
};

struct GE_Engine {
    GE_EngineConfig config;
    GE_Canvas canvas;
    GE_Input input;
    GE_Camera camera;
    GE_GameObject **objects;
    size_t object_count;
    size_t object_capacity;
    int running;
    GE_InitCallback init;
    GE_UpdateCallback update;
    GE_RenderCallback render;
    void *user_data;
};

#endif

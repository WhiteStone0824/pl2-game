#ifndef PL2_GE_GAME_OBJECT_INTERNAL_H
#define PL2_GE_GAME_OBJECT_INTERNAL_H
#include "../../include/game_engine.h"

struct GE_GameObject {
    GE_Engine *engine;
    GE_Transform transform;
    char *name;
    GE_Component **components;
    size_t component_count;
    size_t component_capacity;
    int active;
    int pending_destroy;
};

struct GE_SpriteRenderer {
    GE_Component base;
    GE_Sprite *sprite;
    int visible;
};

struct GE_Animation {
    GE_Sprite **frames;
    double *durations;
    size_t frame_count;
    int loop;
};

struct GE_Animator {
    GE_Component base;
    GE_SpriteRenderer *renderer;
    GE_Animation *animation;
    size_t frame;
    double elapsed;
    int playing;
};

struct GE_Collider {
    GE_Component base;
    GE_ColliderShape shape;
    GE_Rect box;
    GE_Vector2 center;
    float radius;
    GE_Vector2 *points;
    size_t point_count;
    const GE_Sprite *mask;
};

#endif

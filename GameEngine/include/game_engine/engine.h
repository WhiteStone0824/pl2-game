#ifndef PL2_GAME_ENGINE_ENGINE_H
#define PL2_GAME_ENGINE_ENGINE_H
#include "component.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GE_EngineConfig {
    const char *title;
    int target_fps;
    int cell_width;
    int hide_cursor;
    int enable_color;
} GE_EngineConfig;
typedef void (*GE_InitCallback)(GE_Engine *, void *);
typedef void (*GE_UpdateCallback)(GE_Engine *, double delta_seconds, void *);
typedef void (*GE_RenderCallback)(GE_Engine *, void *);

GE_Engine *ge_engine_create(const GE_EngineConfig *config);
void ge_engine_destroy(GE_Engine *engine);
void ge_engine_set_callbacks(GE_Engine *, GE_InitCallback, GE_UpdateCallback,
                             GE_RenderCallback, void *user_data);
int ge_engine_run(GE_Engine *engine);
void ge_engine_quit(GE_Engine *engine);
GE_Canvas *ge_engine_canvas(GE_Engine *engine);
GE_Input *ge_engine_input(GE_Engine *engine);
GE_Camera *ge_engine_camera(GE_Engine *engine);

GE_GameObject *ge_game_object_create(GE_Engine *, const char *name);
void ge_game_object_destroy(GE_GameObject *);
void ge_game_object_set_active(GE_GameObject *, int active);
int ge_game_object_is_active(const GE_GameObject *);
const char *ge_game_object_name(const GE_GameObject *);
GE_Transform *ge_game_object_transform(GE_GameObject *);
GE_Component *ge_game_object_add_component(GE_GameObject *, const GE_ComponentDescriptor *);
GE_Component *ge_game_object_get_component(GE_GameObject *, unsigned type_id);

#ifdef __cplusplus
}
#endif
#endif

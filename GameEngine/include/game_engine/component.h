#ifndef PL2_GAME_ENGINE_COMPONENT_H
#define PL2_GAME_ENGINE_COMPONENT_H
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GE_ComponentStart)(GE_Component *);
typedef void (*GE_ComponentUpdate)(GE_Component *, double delta_seconds);
typedef void (*GE_ComponentDestroy)(GE_Component *);

/* Custom component structs embed GE_Component as their first member. */
typedef struct GE_ComponentDescriptor {
    /* Allocate this many bytes for the component instance. */
    size_t size;
    /* Use a project-specific value at or above GE_COMPONENT_USER. */
    unsigned type_id;
    GE_ComponentStart start;
    GE_ComponentUpdate update;
    GE_ComponentDestroy destroy;
} GE_ComponentDescriptor;

struct GE_Component {
    GE_GameObject *game_object;
    unsigned type_id;
    int enabled;
    void *user_data;
    int started;
    GE_ComponentStart start;
    GE_ComponentUpdate update;
    GE_ComponentDestroy destroy;
};

enum {
    GE_COMPONENT_SPRITE_RENDERER = 1,
    GE_COMPONENT_ANIMATOR,
    GE_COMPONENT_COLLIDER,
    GE_COMPONENT_AUDIO_SOURCE,
    GE_COMPONENT_USER = 1024
};

#ifdef __cplusplus
}
#endif
#endif

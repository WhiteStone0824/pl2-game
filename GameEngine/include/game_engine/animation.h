#ifndef PL2_GAME_ENGINE_ANIMATION_H
#define PL2_GAME_ENGINE_ANIMATION_H
#include "component.h"

#ifdef __cplusplus
extern "C" {
#endif

GE_Animation *ge_animation_create(GE_Sprite *const *frames, const double *durations,
                                  size_t frame_count, int loop);
void ge_animation_destroy(GE_Animation *);
GE_Animator *ge_animator_add(GE_GameObject *);
void ge_animator_play(GE_Animator *, GE_Animation *, int restart);
void ge_animator_stop(GE_Animator *);
size_t ge_animator_frame(const GE_Animator *);

#ifdef __cplusplus
}
#endif
#endif

#include "../internal.h"
#include <stdlib.h>
#include <string.h>

GE_Animation *ge_animation_create(GE_Sprite *const *frames, const double *durations,
                                 size_t frame_count, int loop) {
    if (!frames || frame_count == 0) return NULL;

    GE_Animation *animation = (GE_Animation *)calloc(1, sizeof(*animation));
    if (!animation) return NULL;
    animation->frames = (GE_Sprite **)malloc(frame_count * sizeof(*animation->frames));
    animation->durations = (double *)malloc(frame_count * sizeof(*animation->durations));
    if (!animation->frames || !animation->durations) {
        ge_animation_destroy(animation);
        return NULL;
    }

    animation->frame_count = frame_count;
    animation->loop = !!loop;
    memcpy(animation->frames, frames, frame_count * sizeof(*frames));
    for (size_t i = 0; i < frame_count; ++i) {
        if (!frames[i]) {
            ge_animation_destroy(animation);
            return NULL;
        }
        animation->durations[i] = durations && durations[i] > 0.0 ? durations[i] : 0.1;
    }
    return animation;
}

void ge_animation_destroy(GE_Animation *animation) {
    if (!animation) return;
    free(animation->frames);
    free(animation->durations);
    free(animation);
}

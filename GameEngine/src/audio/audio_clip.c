#include "../internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GE_AudioClip *ge_audio_clip_load(const char *relative_path) {
    if (!ge_path_is_relative_safe(relative_path)) return NULL;

    FILE *file = fopen(relative_path, "rb");
    if (!file) return NULL;
    fclose(file);

    GE_AudioClip *clip = (GE_AudioClip *)calloc(1, sizeof(*clip));
    if (!clip) return NULL;
    size_t length = strlen(relative_path);
    clip->relative_path = (char *)malloc(length + 1);
    if (!clip->relative_path) {
        free(clip);
        return NULL;
    }
    memcpy(clip->relative_path, relative_path, length + 1);
    return clip;
}

void ge_audio_clip_destroy(GE_AudioClip *clip) {
    if (!clip) return;
    free(clip->relative_path);
    free(clip);
}

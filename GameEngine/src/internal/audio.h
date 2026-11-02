#ifndef PL2_GE_AUDIO_INTERNAL_H
#define PL2_GE_AUDIO_INTERNAL_H
#include "../../include/game_engine.h"

struct GE_AudioClip { char *relative_path; };
struct GE_AudioSource {
    GE_Component base;
    GE_AudioClip *clip;
    int loop;
};

#endif

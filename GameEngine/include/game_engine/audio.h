#ifndef PL2_GAME_ENGINE_AUDIO_H
#define PL2_GAME_ENGINE_AUDIO_H
#include "component.h"

#ifdef __cplusplus
extern "C" {
#endif

GE_AudioClip *ge_audio_clip_load(const char *relative_path);
void ge_audio_clip_destroy(GE_AudioClip *);
GE_AudioSource *ge_audio_source_add(GE_GameObject *);
int ge_audio_source_play(GE_AudioSource *, GE_AudioClip *, int loop);
void ge_audio_source_stop(GE_AudioSource *);

#ifdef __cplusplus
}
#endif
#endif

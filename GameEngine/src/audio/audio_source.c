#include "../internal.h"

GE_AudioSource *ge_audio_source_add(GE_GameObject *object) {
    const GE_ComponentDescriptor descriptor = {
        .size = sizeof(GE_AudioSource),
        .type_id = GE_COMPONENT_AUDIO_SOURCE
    };
    GE_AudioSource *source =
        (GE_AudioSource *)ge_game_object_add_component(object, &descriptor);
    return source;
}

int ge_audio_source_play(GE_AudioSource *source, GE_AudioClip *clip, int loop) {
    if (!source || !clip) return 0;
    source->clip = clip;
    source->loop = loop != 0;
    return ge_platform_audio_play(clip->relative_path, source->loop);
}

void ge_audio_source_stop(GE_AudioSource *source) {
    if (!source) return;
    source->loop = 0;
    source->clip = NULL;
    ge_platform_audio_stop();
}

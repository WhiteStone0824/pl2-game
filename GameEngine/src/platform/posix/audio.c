#ifndef _WIN32
#include "../../internal.h"
#include <stdio.h>

int ge_platform_audio_play(const char *path, int loop) {
    (void)path;
    (void)loop;
    fputc('\a', stdout);
    fflush(stdout);
    return 1;
}

void ge_platform_audio_stop(void) { }
#endif

#ifdef _WIN32
#include "../../internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

int ge_platform_audio_play(const char *path, int loop) {
    DWORD flags = SND_ASYNC | SND_FILENAME | SND_NODEFAULT;
    if (loop) flags |= SND_LOOP;
    return PlaySoundA(path, NULL, flags) != 0;
}

void ge_platform_audio_stop(void) { PlaySoundA(NULL, NULL, 0); }
#endif

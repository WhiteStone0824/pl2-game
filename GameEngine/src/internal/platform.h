#ifndef PL2_GE_PLATFORM_INTERNAL_H
#define PL2_GE_PLATFORM_INTERNAL_H
#include "../../include/game_engine.h"

int ge_platform_init(GE_Engine *);
void ge_platform_shutdown(GE_Engine *);
int ge_platform_size(int *, int *);
void ge_platform_poll(GE_Input *);
void ge_platform_sleep(int milliseconds);
double ge_platform_time(void);
void ge_platform_present(GE_Canvas *);
int ge_platform_audio_play(const char *relative_path, int loop);
void ge_platform_audio_stop(void);

#endif

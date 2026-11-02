#ifndef PL2_GE_INTERNAL_H
#define PL2_GE_INTERNAL_H

#include "internal/engine.h"
#include "internal/game_object.h"
#include "internal/audio.h"
#include "internal/save.h"
#include "internal/platform.h"

void ge_game_object_update_components(GE_GameObject *, double delta_seconds);
void ge_game_object_free(GE_GameObject *);
int ge_path_is_relative_safe(const char *path);

#endif

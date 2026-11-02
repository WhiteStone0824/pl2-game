#ifndef PL2_GAME_ENGINE_INPUT_H
#define PL2_GAME_ENGINE_INPUT_H
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

int ge_input_down(const GE_Input *, int key);
int ge_input_pressed(const GE_Input *, int key);

#ifdef __cplusplus
}
#endif
#endif

#include "../internal.h"

int ge_input_down(const GE_Input *input, int key) {
    return input && key >= 0 && key < 512 && input->down[key];
}

int ge_input_pressed(const GE_Input *input, int key) {
    return input && key >= 0 && key < 512 && input->pressed[key];
}

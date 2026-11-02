#ifdef _WIN32
#include "../../internal.h"
#include <conio.h>
#include <string.h>

static void press_key(GE_Input *input, int key) {
    if (key < 0 || key >= 512) return;
    if (!input->previous[key]) input->pressed[key] = 1;
    input->down[key] = 1;
}

void ge_platform_poll(GE_Input *input) {
    memset(input->down, 0, sizeof input->down);
    while (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {
            int scan = _getch();
            if (scan == 72) press_key(input, GE_KEY_UP);
            else if (scan == 80) press_key(input, GE_KEY_DOWN);
            else if (scan == 75) press_key(input, GE_KEY_LEFT);
            else if (scan == 77) press_key(input, GE_KEY_RIGHT);
        } else press_key(input, key);
    }
    memcpy(input->previous, input->down, sizeof input->down);
}
#endif

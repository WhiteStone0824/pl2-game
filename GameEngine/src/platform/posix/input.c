#ifndef _WIN32
#include "../../internal.h"
#include <sys/select.h>
#include <unistd.h>
#include <string.h>

static void press_key(GE_Input *input, int key) {
    if (key < 0 || key >= 512) return;
    if (!input->previous[key]) input->pressed[key] = 1;
    input->down[key] = 1;
}

void ge_platform_poll(GE_Input *input) {
    memset(input->down, 0, sizeof input->down);
    fd_set readable;
    FD_ZERO(&readable);
    FD_SET(STDIN_FILENO, &readable);
    struct timeval timeout = { 0, 0 };

    while (select(STDIN_FILENO + 1, &readable, NULL, NULL, &timeout) > 0) {
        unsigned char key;
        if (read(STDIN_FILENO, &key, 1) != 1) break;
        if (key == 27) {
            unsigned char sequence[2];
            if (read(STDIN_FILENO, sequence, 2) == 2 && sequence[0] == '[') {
                switch (sequence[1]) {
                    case 'A': press_key(input, GE_KEY_UP); break;
                    case 'B': press_key(input, GE_KEY_DOWN); break;
                    case 'C': press_key(input, GE_KEY_RIGHT); break;
                    case 'D': press_key(input, GE_KEY_LEFT); break;
                    default: break;
                }
            } else press_key(input, GE_KEY_ESCAPE);
        } else press_key(input, key);

        FD_ZERO(&readable);
        FD_SET(STDIN_FILENO, &readable);
    }
    memcpy(input->previous, input->down, sizeof input->down);
}
#endif

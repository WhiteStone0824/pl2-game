#ifndef _WIN32
#include "../../internal.h"
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

static struct termios original_terminal;
static int terminal_mode_saved;

int ge_platform_init(GE_Engine *engine) {
    (void)engine;
    if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &original_terminal) == 0) {
        struct termios raw = original_terminal;
        raw.c_lflag &= (tcflag_t)~(ICANON | ECHO);
        raw.c_iflag &= (tcflag_t)~(IXON | ICRNL);
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        terminal_mode_saved = tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0;
    }
    return 1;
}

void ge_platform_shutdown(GE_Engine *engine) {
    if (terminal_mode_saved) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
        terminal_mode_saved = 0;
    }
    if (engine && engine->config.hide_cursor) fputs("\033[0m\033[?25h\n", stdout);
    fflush(stdout);
}

int ge_platform_size(int *width, int *height) {
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col && size.ws_row) {
        *width = size.ws_col;
        *height = size.ws_row;
        return 1;
    }
    *width = 80;
    *height = 24;
    return 0;
}

void ge_platform_sleep(int milliseconds) {
    struct timespec delay = { milliseconds / 1000, (long)(milliseconds % 1000) * 1000000L };
    nanosleep(&delay, NULL);
}

double ge_platform_time(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)now.tv_sec + (double)now.tv_nsec / 1e9;
}

static const char *foreground_code(GE_Color color) {
    static const char *codes[] = { "30", "31", "32", "33", "34", "35", "36", "37" };
    return color >= GE_COLOR_BLACK && color <= GE_COLOR_WHITE ? codes[color] : NULL;
}

void ge_platform_present(GE_Canvas *canvas) {
    if (!canvas || !canvas->cells) return;
    fputs("\033[H", stdout);
    for (int y = 0; y < canvas->height; ++y) {
        for (int x = 0; x < canvas->width; ++x) {
            GE_Cell cell = canvas->cells[y * canvas->width + x];
            if (canvas->enable_color) {
                if (cell.use_background)
                    fprintf(stdout, "\033[48;2;%u;%u;%um", cell.background.r, cell.background.g, cell.background.b);
                else if (foreground_code(cell.color))
                    fprintf(stdout, "\033[%sm", foreground_code(cell.color));
                else fputs("\033[0m", stdout);
                if (cell.use_foreground)
                    fprintf(stdout, "\033[38;2;%u;%u;%um", cell.foreground.r, cell.foreground.g, cell.foreground.b);
            }
            fputc(cell.glyph ? cell.glyph : ' ', stdout);
            for (int pad = 1; pad < canvas->cell_width; ++pad) fputc(' ', stdout);
        }
        fputs("\033[0m\033[K\r\n", stdout);
    }
    fflush(stdout);
}
#endif

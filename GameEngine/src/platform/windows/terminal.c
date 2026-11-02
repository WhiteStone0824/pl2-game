#ifdef _WIN32
#include "../../internal.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

static HANDLE console_output;
static DWORD original_console_mode;
static int console_mode_saved;

int ge_platform_init(GE_Engine *engine) {
    console_output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(console_output, &mode)) {
        original_console_mode = mode;
        console_mode_saved = 1;
        SetConsoleMode(console_output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
    if (engine->config.title) SetConsoleTitleA(engine->config.title);
    if (engine->config.hide_cursor) {
        CONSOLE_CURSOR_INFO cursor;
        if (GetConsoleCursorInfo(console_output, &cursor)) {
            cursor.bVisible = FALSE;
            SetConsoleCursorInfo(console_output, &cursor);
        }
    }
    return 1;
}

void ge_platform_shutdown(GE_Engine *engine) {
    (void)engine;
    if (console_mode_saved) SetConsoleMode(console_output, original_console_mode);
    fputs("\033[0m\033[?25h\n", stdout);
    fflush(stdout);
}

int ge_platform_size(int *width, int *height) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(console_output, &info)) {
        *width = info.srWindow.Right - info.srWindow.Left + 1;
        *height = info.srWindow.Bottom - info.srWindow.Top + 1;
        return 1;
    }
    *width = 80;
    *height = 24;
    return 0;
}

void ge_platform_sleep(int milliseconds) { Sleep((DWORD)milliseconds); }

double ge_platform_time(void) {
    static LARGE_INTEGER frequency;
    LARGE_INTEGER now;
    if (!frequency.QuadPart) QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&now);
    return (double)now.QuadPart / (double)frequency.QuadPart;
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

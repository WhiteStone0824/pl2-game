#include "../internal.h"
#include <string.h>

int ge_path_is_relative_safe(const char *path) {
    if (!path || !*path || path[0] == '/' || path[0] == '\\' || strchr(path, ':')) return 0;

    const char *segment = path;
    for (const char *cursor = path;; ++cursor) {
        if (*cursor != '/' && *cursor != '\\' && *cursor != '\0') continue;
        if (cursor - segment == 2 && segment[0] == '.' && segment[1] == '.') return 0;
        if (*cursor == '\0') break;
        segment = cursor + 1;
    }
    return 1;
}

#include "../internal.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

enum { GE_SAVE_LINE_CAPACITY = 65536 };

static int write_string(FILE *file, const GE_SaveEntry *entry) {
    static const char hex_digits[] = "0123456789ABCDEF";
    if (fprintf(file, "S\t%s\t", entry->key) < 0) return 0;

    const unsigned char *value = (const unsigned char *)entry->value.string;
    for (; *value; ++value) {
        if (fputc(hex_digits[*value >> 4], file) == EOF ||
            fputc(hex_digits[*value & 0x0f], file) == EOF) return 0;
    }
    return fputc('\n', file) != EOF;
}

static int write_entry(FILE *file, const GE_SaveEntry *entry) {
    switch (entry->type) {
        case GE_SAVE_INT:
            return fprintf(file, "I\t%s\t%d\n", entry->key, entry->value.integer) >= 0;
        case GE_SAVE_DOUBLE:
            return fprintf(file, "D\t%s\t%.17g\n", entry->key, entry->value.real) >= 0;
        case GE_SAVE_STRING:
            return write_string(file, entry);
    }
    return 0;
}

int ge_save_write(const GE_SaveData *data, const char *relative_path) {
    if (!data || !ge_path_is_relative_safe(relative_path)) return 0;

    size_t path_length = strlen(relative_path);
    char *temporary_path = (char *)malloc(path_length + sizeof(".tmp"));
    if (!temporary_path) return 0;
    memcpy(temporary_path, relative_path, path_length);
    memcpy(temporary_path + path_length, ".tmp", sizeof(".tmp"));
    remove(temporary_path);

    FILE *file = fopen(temporary_path, "wb");
    if (!file) {
        free(temporary_path);
        return 0;
    }

    int success = fputs("PL2SAVE\t1\n", file) >= 0;
    for (size_t i = 0; success && i < data->count; ++i)
        success = write_entry(file, &data->entries[i]);

    if (fflush(file) != 0) success = 0;
    if (fclose(file) != 0) success = 0;

    if (success) {
#ifdef _WIN32
        success = MoveFileExA(temporary_path, relative_path,
                              MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
        success = rename(temporary_path, relative_path) == 0;
#endif
    }
    if (!success) remove(temporary_path);
    free(temporary_path);
    return success;
}

static int hex_value(char character) {
    if (character >= '0' && character <= '9') return character - '0';
    if (character >= 'A' && character <= 'F') return character - 'A' + 10;
    if (character >= 'a' && character <= 'f') return character - 'a' + 10;
    return -1;
}

static int read_entry(GE_SaveData *data, char *line) {
    char *type = line;
    char *key = strchr(type, '\t');
    if (!key) return 0;
    *key++ = '\0';
    char *value = strchr(key, '\t');
    if (!value) return 0;
    *value++ = '\0';
    char *line_end = strpbrk(value, "\r\n");
    if (line_end) *line_end = '\0';

    if (type[1] != '\0') return 0;
    switch (type[0]) {
        case 'I': {
            char *end = NULL;
            errno = 0;
            long parsed = strtol(value, &end, 10);
            if (errno || end == value || *end || parsed < INT_MIN || parsed > INT_MAX) return 0;
            return ge_save_set_int(data, key, (int)parsed);
        }
        case 'D': {
            char *end = NULL;
            errno = 0;
            double parsed = strtod(value, &end);
            if (errno || end == value || *end) return 0;
            return ge_save_set_double(data, key, parsed);
        }
        case 'S': {
            size_t encoded_length = strlen(value);
            if (encoded_length % 2 != 0) return 0;
            char *decoded = (char *)malloc(encoded_length / 2 + 1);
            if (!decoded) return 0;
            for (size_t i = 0; i < encoded_length; i += 2) {
                int high = hex_value(value[i]);
                int low = hex_value(value[i + 1]);
                if (high < 0 || low < 0) {
                    free(decoded);
                    return 0;
                }
                decoded[i / 2] = (char)((high << 4) | low);
            }
            decoded[encoded_length / 2] = '\0';
            int success = ge_save_set_string(data, key, decoded);
            free(decoded);
            return success;
        }
        default:
            return 0;
    }
}

GE_SaveData *ge_save_read(const char *relative_path) {
    if (!ge_path_is_relative_safe(relative_path)) return NULL;
    FILE *file = fopen(relative_path, "rb");
    if (!file) return NULL;

    char line[GE_SAVE_LINE_CAPACITY];
    if (!fgets(line, sizeof(line), file) || strcmp(line, "PL2SAVE\t1\n") != 0) {
        fclose(file);
        return NULL;
    }

    GE_SaveData *data = ge_save_data_create();
    if (!data) {
        fclose(file);
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        if (!strchr(line, '\n') && !feof(file)) goto fail;
        if (!read_entry(data, line)) goto fail;
    }
    if (ferror(file)) goto fail;

    fclose(file);
    return data;

fail:
    fclose(file);
    ge_save_data_destroy(data);
    return NULL;
}

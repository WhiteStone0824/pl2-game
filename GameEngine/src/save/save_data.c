#include "../internal.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static int valid_key(const char *key) {
    if (!key || !*key) return 0;
    for (const unsigned char *p = (const unsigned char *)key; *p; ++p) {
        if (!(isalnum(*p) || *p == '_' || *p == '-' || *p == '.')) return 0;
    }
    return 1;
}

static GE_SaveEntry *find_entry(GE_SaveData *data, const char *key) {
    if (!data || !key) return NULL;
    for (size_t i = 0; i < data->count; ++i) {
        if (strcmp(data->entries[i].key, key) == 0) return &data->entries[i];
    }
    return NULL;
}

static GE_SaveEntry *get_or_add_entry(GE_SaveData *data, const char *key, GE_SaveType type) {
    if (!data || !valid_key(key)) return NULL;

    GE_SaveEntry *entry = find_entry(data, key);
    if (entry) {
        if (entry->type == GE_SAVE_STRING) free(entry->value.string);
        memset(&entry->value, 0, sizeof(entry->value));
        entry->type = type;
        return entry;
    }

    if (data->count == data->capacity) {
        size_t capacity = data->capacity ? data->capacity * 2 : 16;
        GE_SaveEntry *entries = (GE_SaveEntry *)realloc(
            data->entries, capacity * sizeof(*entries));
        if (!entries) return NULL;
        data->entries = entries;
        data->capacity = capacity;
    }

    entry = &data->entries[data->count];
    memset(entry, 0, sizeof(*entry));
    size_t key_length = strlen(key);
    entry->key = (char *)malloc(key_length + 1);
    if (!entry->key) return NULL;
    memcpy(entry->key, key, key_length + 1);
    entry->type = type;
    ++data->count;
    return entry;
}

GE_SaveData *ge_save_data_create(void) {
    return (GE_SaveData *)calloc(1, sizeof(GE_SaveData));
}

void ge_save_data_destroy(GE_SaveData *data) {
    if (!data) return;
    for (size_t i = 0; i < data->count; ++i) {
        free(data->entries[i].key);
        if (data->entries[i].type == GE_SAVE_STRING) free(data->entries[i].value.string);
    }
    free(data->entries);
    free(data);
}

int ge_save_set_int(GE_SaveData *data, const char *key, int value) {
    GE_SaveEntry *entry = get_or_add_entry(data, key, GE_SAVE_INT);
    if (!entry) return 0;
    entry->value.integer = value;
    return 1;
}

int ge_save_set_double(GE_SaveData *data, const char *key, double value) {
    GE_SaveEntry *entry = get_or_add_entry(data, key, GE_SAVE_DOUBLE);
    if (!entry) return 0;
    entry->value.real = value;
    return 1;
}

int ge_save_set_string(GE_SaveData *data, const char *key, const char *value) {
    if (!value) return 0;
    size_t length = strlen(value);
    char *copy = (char *)malloc(length + 1);
    if (!copy) return 0;
    memcpy(copy, value, length + 1);

    GE_SaveEntry *entry = get_or_add_entry(data, key, GE_SAVE_STRING);
    if (!entry) {
        free(copy);
        return 0;
    }
    entry->value.string = copy;
    return 1;
}

int ge_save_get_int(const GE_SaveData *data, const char *key, int *value) {
    if (!data || !key || !value) return 0;
    for (size_t i = 0; i < data->count; ++i) {
        const GE_SaveEntry *entry = &data->entries[i];
        if (strcmp(entry->key, key) == 0 && entry->type == GE_SAVE_INT) {
            *value = entry->value.integer;
            return 1;
        }
    }
    return 0;
}

int ge_save_get_double(const GE_SaveData *data, const char *key, double *value) {
    if (!data || !key || !value) return 0;
    for (size_t i = 0; i < data->count; ++i) {
        const GE_SaveEntry *entry = &data->entries[i];
        if (strcmp(entry->key, key) == 0 && entry->type == GE_SAVE_DOUBLE) {
            *value = entry->value.real;
            return 1;
        }
    }
    return 0;
}

const char *ge_save_get_string(const GE_SaveData *data, const char *key) {
    if (!data || !key) return NULL;
    for (size_t i = 0; i < data->count; ++i) {
        const GE_SaveEntry *entry = &data->entries[i];
        if (strcmp(entry->key, key) == 0 && entry->type == GE_SAVE_STRING)
            return entry->value.string;
    }
    return NULL;
}

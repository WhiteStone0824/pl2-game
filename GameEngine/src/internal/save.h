#ifndef PL2_GE_SAVE_INTERNAL_H
#define PL2_GE_SAVE_INTERNAL_H
#include "../../include/game_engine.h"

typedef enum GE_SaveType { GE_SAVE_INT, GE_SAVE_DOUBLE, GE_SAVE_STRING } GE_SaveType;
typedef struct GE_SaveEntry {
    char *key;
    GE_SaveType type;
    union { int integer; double real; char *string; } value;
} GE_SaveEntry;

struct GE_SaveData {
    GE_SaveEntry *entries;
    size_t count;
    size_t capacity;
};

#endif

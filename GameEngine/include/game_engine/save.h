#ifndef PL2_GAME_ENGINE_SAVE_H
#define PL2_GAME_ENGINE_SAVE_H
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

GE_SaveData *ge_save_data_create(void);
void ge_save_data_destroy(GE_SaveData *);
int ge_save_set_int(GE_SaveData *, const char *key, int value);
int ge_save_set_double(GE_SaveData *, const char *key, double value);
int ge_save_set_string(GE_SaveData *, const char *key, const char *value);
int ge_save_get_int(const GE_SaveData *, const char *key, int *value);
int ge_save_get_double(const GE_SaveData *, const char *key, double *value);
const char *ge_save_get_string(const GE_SaveData *, const char *key);
int ge_save_write(const GE_SaveData *, const char *relative_path);
GE_SaveData *ge_save_read(const char *relative_path);

#ifdef __cplusplus
}
#endif
#endif

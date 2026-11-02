#include "../internal.h"

void ge_camera_set(GE_Camera *camera, float x, float y) {
    if (!camera) return;
    camera->x = x;
    camera->y = y;
}

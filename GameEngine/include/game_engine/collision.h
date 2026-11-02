#ifndef PL2_GAME_ENGINE_COLLISION_H
#define PL2_GAME_ENGINE_COLLISION_H
#include "component.h"

#ifdef __cplusplus
extern "C" {
#endif

GE_Collider *ge_collider_add(GE_GameObject *, GE_ColliderShape);
void ge_collider_set_box(GE_Collider *, GE_Rect local_bounds);
void ge_collider_set_circle(GE_Collider *, GE_Vector2 local_center, float radius);
int ge_collider_set_polygon(GE_Collider *, const GE_Vector2 *points, size_t count);
void ge_collider_set_pixel_mask(GE_Collider *, const GE_Sprite *mask);
int ge_collider_intersects(const GE_Collider *, const GE_Collider *);

#ifdef __cplusplus
}
#endif
#endif

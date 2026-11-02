#include "../internal.h"
#include <stdlib.h>

static void collider_destroy(GE_Component *component) {
    GE_Collider *collider = (GE_Collider *)component;
    free(collider->points);
}

GE_Collider *ge_collider_add(GE_GameObject *object, GE_ColliderShape shape) {
    const GE_ComponentDescriptor descriptor = {
        .size = sizeof(GE_Collider),
        .type_id = GE_COMPONENT_COLLIDER,
        .destroy = collider_destroy
    };
    GE_Collider *collider = (GE_Collider *)ge_game_object_add_component(object, &descriptor);
    if (!collider) return NULL;
    collider->shape = shape;
    collider->box = (GE_Rect){ 0.0f, 0.0f, 1.0f, 1.0f };
    collider->radius = 0.5f;
    return collider;
}

void ge_collider_set_box(GE_Collider *collider, GE_Rect local_bounds) {
    if (!collider) return;
    collider->shape = GE_COLLIDER_BOX;
    collider->box = local_bounds;
}

void ge_collider_set_circle(GE_Collider *collider, GE_Vector2 local_center, float radius) {
    if (!collider) return;
    collider->shape = GE_COLLIDER_CIRCLE;
    collider->center = local_center;
    collider->radius = radius > 0.0f ? radius : 0.0f;
}

int ge_collider_set_polygon(GE_Collider *collider, const GE_Vector2 *points, size_t count) {
    if (!collider || !points || count < 3) return 0;
    GE_Vector2 *copy = (GE_Vector2 *)malloc(count * sizeof(*copy));
    if (!copy) return 0;
    for (size_t i = 0; i < count; ++i) copy[i] = points[i];

    free(collider->points);
    collider->points = copy;
    collider->point_count = count;
    collider->shape = GE_COLLIDER_POLYGON;
    return 1;
}

void ge_collider_set_pixel_mask(GE_Collider *collider, const GE_Sprite *mask) {
    if (!collider) return;
    collider->shape = GE_COLLIDER_PIXEL_MASK;
    collider->mask = mask;
}

#include "../internal.h"
#include <math.h>

static GE_Vector2 world_to_collider_local(const GE_Collider *collider, float x, float y) {
    const GE_Transform *transform = &collider->base.game_object->transform;
    float dx = x - transform->x;
    float dy = y - transform->y;
    float radians = -transform->rotation * 0.01745329251994329577f;
    float cosine = cosf(radians);
    float sine = sinf(radians);
    float scale_x = transform->scale_x == 0.0f ? 1.0f : transform->scale_x;
    float scale_y = transform->scale_y == 0.0f ? 1.0f : transform->scale_y;
    return (GE_Vector2){
        (dx * cosine - dy * sine) / scale_x,
        (dx * sine + dy * cosine) / scale_y
    };
}

static int point_in_polygon(const GE_Vector2 *points, size_t count, float x, float y) {
    int inside = 0;
    for (size_t i = 0, j = count - 1; i < count; j = i++) {
        int crosses_scanline = (points[i].y > y) != (points[j].y > y);
        if (!crosses_scanline) continue;
        float intersection_x = (points[j].x - points[i].x) *
                              (y - points[i].y) / (points[j].y - points[i].y) + points[i].x;
        if (x < intersection_x) inside = !inside;
    }
    return inside;
}

static int collider_contains_point(const GE_Collider *collider, float x, float y) {
    GE_Vector2 point = world_to_collider_local(collider, x, y);
    switch (collider->shape) {
        case GE_COLLIDER_BOX:
            return point.x >= collider->box.x && point.y >= collider->box.y &&
                   point.x < collider->box.x + collider->box.width &&
                   point.y < collider->box.y + collider->box.height;
        case GE_COLLIDER_CIRCLE: {
            float dx = point.x - collider->center.x;
            float dy = point.y - collider->center.y;
            return dx * dx + dy * dy <= collider->radius * collider->radius;
        }
        case GE_COLLIDER_POLYGON:
            return collider->points && point_in_polygon(
                collider->points, collider->point_count, point.x, point.y);
        case GE_COLLIDER_PIXEL_MASK: {
            if (!collider->mask || !collider->mask->cells) return 0;
            int pixel_x = (int)floorf(point.x);
            int pixel_y = (int)floorf(point.y);
            if (pixel_x < 0 || pixel_y < 0 || pixel_x >= collider->mask->width ||
                pixel_y >= collider->mask->height) return 0;
            GE_Cell cell = collider->mask->cells[pixel_y * collider->mask->width + pixel_x];
            return cell.color != GE_COLOR_TRANSPARENT || cell.use_background;
        }
    }
    return 0;
}

static GE_Rect collider_bounds(const GE_Collider *collider) {
    GE_Rect local = { 0.0f, 0.0f, 1.0f, 1.0f };
    if (collider->shape == GE_COLLIDER_BOX) {
        local = collider->box;
    } else if (collider->shape == GE_COLLIDER_CIRCLE) {
        local = (GE_Rect){
            collider->center.x - collider->radius,
            collider->center.y - collider->radius,
            2.0f * collider->radius,
            2.0f * collider->radius
        };
    } else if (collider->shape == GE_COLLIDER_PIXEL_MASK && collider->mask) {
        local = (GE_Rect){ 0.0f, 0.0f, (float)collider->mask->width, (float)collider->mask->height };
    } else if (collider->shape == GE_COLLIDER_POLYGON && collider->points && collider->point_count) {
        float min_x = collider->points[0].x;
        float max_x = min_x;
        float min_y = collider->points[0].y;
        float max_y = min_y;
        for (size_t i = 1; i < collider->point_count; ++i) {
            GE_Vector2 point = collider->points[i];
            if (point.x < min_x) min_x = point.x;
            if (point.x > max_x) max_x = point.x;
            if (point.y < min_y) min_y = point.y;
            if (point.y > max_y) max_y = point.y;
        }
        local = (GE_Rect){ min_x, min_y, max_x - min_x, max_y - min_y };
    }

    const GE_Transform *transform = &collider->base.game_object->transform;
    float x[4] = { local.x, local.x + local.width, local.x, local.x + local.width };
    float y[4] = { local.y, local.y, local.y + local.height, local.y + local.height };
    float radians = transform->rotation * 0.01745329251994329577f;
    float cosine = cosf(radians);
    float sine = sinf(radians);
    float min_x = INFINITY, min_y = INFINITY, max_x = -INFINITY, max_y = -INFINITY;

    for (int i = 0; i < 4; ++i) {
        float scaled_x = x[i] * transform->scale_x;
        float scaled_y = y[i] * transform->scale_y;
        float world_x = transform->x + scaled_x * cosine - scaled_y * sine;
        float world_y = transform->y + scaled_x * sine + scaled_y * cosine;
        if (world_x < min_x) min_x = world_x;
        if (world_x > max_x) max_x = world_x;
        if (world_y < min_y) min_y = world_y;
        if (world_y > max_y) max_y = world_y;
    }
    return (GE_Rect){ min_x, min_y, max_x - min_x, max_y - min_y };
}

int ge_collider_intersects(const GE_Collider *a, const GE_Collider *b) {
    if (!a || !b || !a->base.game_object || !b->base.game_object ||
        !ge_game_object_is_active(a->base.game_object) ||
        !ge_game_object_is_active(b->base.game_object)) return 0;

    GE_Rect bounds_a = collider_bounds(a);
    GE_Rect bounds_b = collider_bounds(b);
    float left = fmaxf(bounds_a.x, bounds_b.x);
    float top = fmaxf(bounds_a.y, bounds_b.y);
    float right = fminf(bounds_a.x + bounds_a.width, bounds_b.x + bounds_b.width);
    float bottom = fminf(bounds_a.y + bounds_a.height, bounds_b.y + bounds_b.height);
    if (left >= right || top >= bottom) return 0;

    float step = a->shape == GE_COLLIDER_PIXEL_MASK || b->shape == GE_COLLIDER_PIXEL_MASK
        ? 1.0f : 0.25f;
    for (float y = top + step * 0.5f; y < bottom; y += step) {
        for (float x = left + step * 0.5f; x < right; x += step) {
            if (collider_contains_point(a, x, y) && collider_contains_point(b, x, y)) return 1;
        }
    }
    return 0;
}

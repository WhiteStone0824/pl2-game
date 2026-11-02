#include "../internal.h"
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *source) {
    size_t length = source ? strlen(source) : 0;
    char *copy = (char *)malloc(length + 1);
    if (!copy) return NULL;
    if (length) memcpy(copy, source, length);
    copy[length] = '\0';
    return copy;
}

GE_GameObject *ge_game_object_create(GE_Engine *engine, const char *name) {
    if (!engine) return NULL;

    if (engine->object_count == engine->object_capacity) {
        size_t capacity = engine->object_capacity ? engine->object_capacity * 2 : 16;
        GE_GameObject **objects = (GE_GameObject **)realloc(
            engine->objects, capacity * sizeof(*objects));
        if (!objects) return NULL;
        engine->objects = objects;
        engine->object_capacity = capacity;
    }

    GE_GameObject *object = (GE_GameObject *)calloc(1, sizeof(*object));
    if (!object) return NULL;
    object->name = copy_string(name ? name : "GameObject");
    if (!object->name) {
        free(object);
        return NULL;
    }

    object->engine = engine;
    object->active = 1;
    object->transform.scale_x = 1.0f;
    object->transform.scale_y = 1.0f;
    engine->objects[engine->object_count++] = object;
    return object;
}

void ge_game_object_free(GE_GameObject *object) {
    if (!object) return;
    for (size_t i = 0; i < object->component_count; ++i) {
        GE_Component *component = object->components[i];
        if (component->destroy) component->destroy(component);
        free(component);
    }
    free(object->components);
    free(object->name);
    free(object);
}

void ge_game_object_destroy(GE_GameObject *object) {
    if (object) object->pending_destroy = 1;
}

void ge_game_object_set_active(GE_GameObject *object, int active) {
    if (object) object->active = !!active;
}

int ge_game_object_is_active(const GE_GameObject *object) {
    return object && object->active && !object->pending_destroy;
}

const char *ge_game_object_name(const GE_GameObject *object) {
    return object ? object->name : "";
}

GE_Transform *ge_game_object_transform(GE_GameObject *object) {
    return object ? &object->transform : NULL;
}

GE_Component *ge_game_object_add_component(
    GE_GameObject *object, const GE_ComponentDescriptor *descriptor) {
    if (!object || !descriptor || descriptor->size < sizeof(GE_Component) ||
        object->pending_destroy) return NULL;

    if (object->component_count == object->component_capacity) {
        size_t capacity = object->component_capacity ? object->component_capacity * 2 : 4;
        GE_Component **components = (GE_Component **)realloc(
            object->components, capacity * sizeof(*components));
        if (!components) return NULL;
        object->components = components;
        object->component_capacity = capacity;
    }

    GE_Component *component = (GE_Component *)calloc(1, descriptor->size);
    if (!component) return NULL;
    component->game_object = object;
    component->type_id = descriptor->type_id;
    component->enabled = 1;
    component->start = descriptor->start;
    component->update = descriptor->update;
    component->destroy = descriptor->destroy;
    object->components[object->component_count++] = component;
    return component;
}

GE_Component *ge_game_object_get_component(GE_GameObject *object, unsigned type_id) {
    if (!object) return NULL;
    for (size_t i = 0; i < object->component_count; ++i) {
        if (object->components[i]->type_id == type_id) return object->components[i];
    }
    return NULL;
}

void ge_game_object_update_components(GE_GameObject *object, double delta_seconds) {
    if (!ge_game_object_is_active(object)) return;
    size_t component_count = object->component_count;
    for (size_t i = 0; i < component_count; ++i) {
        GE_Component *component = object->components[i];
        if (!component->enabled) continue;
        if (!component->started) {
            component->started = 1;
            if (component->start) component->start(component);
        }
        if (component->enabled && component->update)
            component->update(component, delta_seconds);
    }
}

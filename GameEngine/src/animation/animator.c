#include "../internal.h"

static void animator_update(GE_Component *component, double delta_seconds) {
    GE_Animator *animator = (GE_Animator *)component;
    if (!animator->playing || !animator->animation || animator->animation->frame_count == 0) return;

    if (!animator->renderer) {
        animator->renderer = (GE_SpriteRenderer *)ge_game_object_get_component(
            component->game_object, GE_COMPONENT_SPRITE_RENDERER);
    }

    animator->elapsed += delta_seconds;
    size_t advance_count = 0;
    while (animator->playing &&
           animator->elapsed >= animator->animation->durations[animator->frame] &&
           advance_count++ < 256) {
        animator->elapsed -= animator->animation->durations[animator->frame];
        if (animator->frame + 1 < animator->animation->frame_count) {
            ++animator->frame;
        } else if (animator->animation->loop) {
            animator->frame = 0;
        } else {
            animator->playing = 0;
            break;
        }

        if (animator->renderer) {
            ge_sprite_renderer_set_sprite(animator->renderer,
                                          animator->animation->frames[animator->frame]);
        }
    }
}

GE_Animator *ge_animator_add(GE_GameObject *object) {
    const GE_ComponentDescriptor descriptor = {
        .size = sizeof(GE_Animator),
        .type_id = GE_COMPONENT_ANIMATOR,
        .update = animator_update
    };
    GE_Animator *animator = (GE_Animator *)ge_game_object_add_component(object, &descriptor);
    if (animator) {
        animator->renderer = (GE_SpriteRenderer *)ge_game_object_get_component(
            object, GE_COMPONENT_SPRITE_RENDERER);
    }
    return animator;
}

void ge_animator_play(GE_Animator *animator, GE_Animation *animation, int restart) {
    if (!animator || !animation || animation->frame_count == 0) return;
    if (!restart && animator->animation == animation && animator->playing) return;

    animator->animation = animation;
    animator->frame = 0;
    animator->elapsed = 0.0;
    animator->playing = 1;
    if (!animator->renderer) {
        animator->renderer = (GE_SpriteRenderer *)ge_game_object_get_component(
            animator->base.game_object, GE_COMPONENT_SPRITE_RENDERER);
    }
    if (animator->renderer) {
        ge_sprite_renderer_set_sprite(animator->renderer, animation->frames[0]);
    }
}

void ge_animator_stop(GE_Animator *animator) {
    if (animator) animator->playing = 0;
}

size_t ge_animator_frame(const GE_Animator *animator) {
    return animator ? animator->frame : 0;
}

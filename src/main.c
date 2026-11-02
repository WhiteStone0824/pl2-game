#include "game_engine.h"

#include <stdio.h>

typedef struct DemoState {
    GE_Sprite player_frames[3];
    GE_Sprite target_sprite;
    GE_Animation *animation;
    GE_GameObject *player;
    GE_GameObject *target;
    GE_Animator *animator;
    GE_Collider *player_collider;
    GE_Collider *target_collider;
    GE_AudioClip *beep;
    GE_AudioSource *audio_source;
    GE_SaveData *save_data;
    int score;
    int overlapping;
} DemoState;

static GE_ColorRGB make_rgb(unsigned char red, unsigned char green, unsigned char blue) {
    GE_ColorRGB color = { red, green, blue };
    return color;
}

static void make_disc(GE_Sprite *sprite, unsigned char color_phase) {
    ge_sprite_init(sprite, 12, 12);
    for (int y = 0; y < sprite->height; ++y) {
        for (int x = 0; x < sprite->width; ++x) {
            int offset_x = x - 5;
            int offset_y = y - 5;
            if (offset_x * offset_x + offset_y * offset_y > 25) continue;

            GE_ColorRGB color = make_rgb(
                (unsigned char)(50 + x * 15),
                (unsigned char)(50 + y * 14),
                (unsigned char)(180 + color_phase * 20));
            ge_sprite_set_pixel(sprite, x, y, color);
        }
    }
}

static void create_player(DemoState *demo, GE_Engine *engine) {
    for (unsigned char frame = 0; frame < 3; ++frame)
        make_disc(&demo->player_frames[frame], frame);

    GE_Sprite *frames[] = {
        &demo->player_frames[0],
        &demo->player_frames[1],
        &demo->player_frames[2]
    };
    const double frame_durations[] = { 0.18, 0.18, 0.18 };
    demo->animation = ge_animation_create(frames, frame_durations, 3, 1);

    demo->player = ge_game_object_create(engine, "Player");
    if (!demo->player) return;
    GE_Transform *transform = ge_game_object_transform(demo->player);
    transform->x = 8.0f;
    transform->y = 10.0f;

    GE_SpriteRenderer *renderer = ge_sprite_renderer_add(demo->player);
    ge_sprite_renderer_set_sprite(renderer, &demo->player_frames[0]);
    demo->animator = ge_animator_add(demo->player);
    ge_animator_play(demo->animator, demo->animation, 1);

    demo->player_collider = ge_collider_add(demo->player, GE_COLLIDER_PIXEL_MASK);
    ge_collider_set_pixel_mask(demo->player_collider, &demo->player_frames[0]);
    demo->audio_source = ge_audio_source_add(demo->player);
}

static void create_target(DemoState *demo, GE_Engine *engine) {
    make_disc(&demo->target_sprite, 2);
    demo->target = ge_game_object_create(engine, "Target");
    if (!demo->target) return;

    GE_Transform *transform = ge_game_object_transform(demo->target);
    transform->x = 40.0f;
    transform->y = 10.0f;
    GE_SpriteRenderer *renderer = ge_sprite_renderer_add(demo->target);
    ge_sprite_renderer_set_sprite(renderer, &demo->target_sprite);

    demo->target_collider = ge_collider_add(demo->target, GE_COLLIDER_POLYGON);
    const GE_Vector2 points[] = { {0, 0}, {12, 2}, {8, 12}, {2, 10} };
    ge_collider_set_polygon(demo->target_collider, points, 4);
}

static void initialize_demo(GE_Engine *engine, void *user_data) {
    DemoState *demo = (DemoState *)user_data;
    create_player(demo, engine);
    create_target(demo, engine);
    demo->beep = ge_audio_clip_load("GameData/Assets/beep.wav");
    demo->save_data = ge_save_data_create();
    ge_save_set_int(demo->save_data, "score", 0);
}

static void handle_save_input(DemoState *demo, const GE_Input *input) {
    if (ge_input_pressed(input, 'p') || ge_input_pressed(input, 'P')) {
        ge_save_set_int(demo->save_data, "score", demo->score);
        ge_save_write(demo->save_data, "GameData/Saves/demo.sav");
    }
    if (ge_input_pressed(input, 'o') || ge_input_pressed(input, 'O')) {
        GE_SaveData *loaded = ge_save_read("GameData/Saves/demo.sav");
        if (!loaded) return;
        ge_save_get_int(loaded, "score", &demo->score);
        ge_save_data_destroy(loaded);
    }
}

static void update_demo(GE_Engine *engine, double delta_seconds, void *user_data) {
    (void)delta_seconds;
    DemoState *demo = (DemoState *)user_data;
    GE_Input *input = ge_engine_input(engine);
    GE_Transform *transform = ge_game_object_transform(demo->player);
    if (!transform) return;

    if (ge_input_down(input, GE_KEY_LEFT) || ge_input_down(input, 'a')) transform->x -= 1.0f;
    if (ge_input_down(input, GE_KEY_RIGHT) || ge_input_down(input, 'd')) transform->x += 1.0f;
    if (ge_input_down(input, GE_KEY_UP) || ge_input_down(input, 'w')) transform->y -= 1.0f;
    if (ge_input_down(input, GE_KEY_DOWN) || ge_input_down(input, 's')) transform->y += 1.0f;

    if (ge_input_pressed(input, ' ')) {
        if (!demo->audio_source || !demo->beep ||
            !ge_audio_source_play(demo->audio_source, demo->beep, 0)) fputc('\a', stdout);
    }
    handle_save_input(demo, input);

    if (ge_input_pressed(input, 'q') || ge_input_pressed(input, 'Q') ||
        ge_input_pressed(input, GE_KEY_ESCAPE)) ge_engine_quit(engine);

    int overlapping = ge_collider_intersects(demo->player_collider, demo->target_collider);
    if (overlapping && !demo->overlapping) ++demo->score;
    demo->overlapping = overlapping;
}

static void render_demo(GE_Engine *engine, void *user_data) {
    DemoState *demo = (DemoState *)user_data;
    GE_Canvas *canvas = ge_engine_canvas(engine);
    ge_canvas_clear(canvas, GE_COLOR_DEFAULT);
    ge_canvas_text(canvas, 1, 1, "COMPONENT ENGINE DEMO", GE_COLOR_WHITE);
    ge_canvas_text(canvas, 1, 3, "ARROWS: move  SPACE: sound  P: save  O: load  Q: quit", GE_COLOR_CYAN);
    ge_canvas_textf(canvas, 1, 4, GE_COLOR_YELLOW, "Score: %d   Shape overlap: %s",
                    demo->score, demo->overlapping ? "yes" : "no");
}

int main(void) {
    DemoState demo = {0};
    GE_EngineConfig config = {
        .title = "PL2 CUI Game Engine",
        .target_fps = 20,
        .cell_width = 2,
        .hide_cursor = 1,
        .enable_color = 1
    };
    GE_Engine *engine = ge_engine_create(&config);
    if (!engine) {
        fprintf(stderr, "Could not initialize an ANSI truecolor terminal.\n");
        return 1;
    }

    ge_engine_set_callbacks(engine, initialize_demo, update_demo, render_demo, &demo);
    int result = ge_engine_run(engine);
    if (demo.audio_source) ge_audio_source_stop(demo.audio_source);
    ge_engine_destroy(engine);
    ge_animation_destroy(demo.animation);
    ge_audio_clip_destroy(demo.beep);
    ge_save_data_destroy(demo.save_data);
    for (int i = 0; i < 3; ++i) ge_sprite_destroy(&demo.player_frames[i]);
    ge_sprite_destroy(&demo.target_sprite);
    return result;
}

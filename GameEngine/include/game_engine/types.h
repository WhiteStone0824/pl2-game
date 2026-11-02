#ifndef PL2_GAME_ENGINE_TYPES_H
#define PL2_GAME_ENGINE_TYPES_H

#include <stddef.h>

typedef enum GE_Color {
    GE_COLOR_DEFAULT = -1,
    GE_COLOR_BLACK,
    GE_COLOR_RED,
    GE_COLOR_GREEN,
    GE_COLOR_YELLOW,
    GE_COLOR_BLUE,
    GE_COLOR_MAGENTA,
    GE_COLOR_CYAN,
    GE_COLOR_WHITE,
    GE_COLOR_TRANSPARENT
} GE_Color;

typedef struct GE_ColorRGB { unsigned char r, g, b; } GE_ColorRGB;
typedef struct GE_Cell {
    char glyph;
    GE_Color color;
    GE_ColorRGB foreground;
    GE_ColorRGB background;
    unsigned char use_background;
    unsigned char use_foreground;
} GE_Cell;

typedef struct GE_Sprite { int width, height; GE_Cell *cells; } GE_Sprite;
typedef struct GE_Transform {
    float x, y, scale_x, scale_y;
    float rotation; /* Degrees, clockwise. */
} GE_Transform;
typedef struct GE_Rect { float x, y, width, height; } GE_Rect;
typedef struct GE_Vector2 { float x, y; } GE_Vector2;
typedef struct GE_Camera { float x, y; } GE_Camera;
typedef enum GE_Key {
    GE_KEY_LEFT = 256, GE_KEY_RIGHT, GE_KEY_UP, GE_KEY_DOWN, GE_KEY_ESCAPE
} GE_Key;
typedef enum GE_ColliderShape {
    GE_COLLIDER_BOX, GE_COLLIDER_CIRCLE, GE_COLLIDER_POLYGON, GE_COLLIDER_PIXEL_MASK
} GE_ColliderShape;

typedef struct GE_Canvas GE_Canvas;
typedef struct GE_Input GE_Input;
typedef struct GE_Engine GE_Engine;
typedef struct GE_GameObject GE_GameObject;
typedef struct GE_Component GE_Component;
typedef struct GE_SpriteRenderer GE_SpriteRenderer;
typedef struct GE_Animation GE_Animation;
typedef struct GE_Animator GE_Animator;
typedef struct GE_Collider GE_Collider;
typedef struct GE_AudioSource GE_AudioSource;
typedef struct GE_AudioClip GE_AudioClip;
typedef struct GE_SaveData GE_SaveData;

#endif

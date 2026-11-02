/*view.h*/

#include <stdio.h>

#define SCREEN_BUFFER_SIZE_WIDTH 317  // 634文字分
#define SCREEN_BUFFER_SIZE_HEIGHT 128 // 128行

#define R "\033[41m\033[31m  \033[0m" //赤
#define G "\033[42m\033[32m  \033[0m" //緑
#define B "\033[44m\033[34m  \033[0m" //青

#define Y "\033[43m\033[33m  \033[0m" //黄色
#define M "\033[45m\033[35m  \033[0m" //マゼンタ
#define C "\033[46m\033[36m  \033[0m" //シアン

#define W "\033[47m\033[37m  \033[0m" //白
#define K "\033[40m\033[30m  \033[0m" //黒
#define _ "  " //透明色。無視する

typedef enum Color
{
    red,
    Green,
    Blue,
    Yellow,
    Magenta,
    Cyan,
    White,
    Black,
    Space
} Color;

typedef struct{
    int x,y;
}Vector2Int;

typedef struct{
    Color map[16][16];
} PixelArt;

typedef struct{
    Vector2Int pos;
    PixelArt *objectView;
} GameObject;

//画面に表示するものを一旦bufferに格納しておく

//bufferを削除する
void clear_screen_buffer(Color *screen_buffer);

//bufferに書き込む
void draw_to_object(const GameObject *o,Color *screen_buffer);

//bufferの内容を表示する
void render_screen(const Color *screen_buffer);
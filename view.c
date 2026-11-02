/*view.c*/

#include "view.h"

void clear_screen_buffer(Color *screen_buffer) {
    for (int i = 0; i < SCREEN_BUFFER_SIZE_WIDTH * SCREEN_BUFFER_SIZE_HEIGHT; i++) {
        screen_buffer[i] = Space;
    }
}

void draw_to_object(const GameObject *o, Color *screen_buffer) {
    if (o == NULL || o->objectView == NULL) return;

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            Color c = o->objectView->map[y][x];
            
            // 透明色（Space）は描画しない
            if (c == Space) continue;

            int screen_x = o->pos.x + x;
            int screen_y = o->pos.y + y;

            // スクリーンバッファの範囲内かチェック（クリッピング）
            if (screen_x >= 0 && screen_x < SCREEN_BUFFER_SIZE_WIDTH &&
                screen_y >= 0 && screen_y < SCREEN_BUFFER_SIZE_HEIGHT) {
                
                screen_buffer[screen_y * SCREEN_BUFFER_SIZE_WIDTH + screen_x] = c;
            }
        }
    }
}

void render_screen(const Color *screen_buffer) {
    // 画面のちらつきを抑えるため、カーソルを左上に移動するシーケンスを出力
    printf("\033[2J\033[H");

    for (int y = 0; y < SCREEN_BUFFER_SIZE_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_BUFFER_SIZE_WIDTH; x++) {
            Color c = screen_buffer[y * SCREEN_BUFFER_SIZE_WIDTH + x];

            // 色に応じたANSIマクロを出力する
            switch (c) {
                case red:     printf(R); break;
                case Green:   printf(G); break;
                case Blue:    printf(B); break;
                case Yellow:  printf(Y); break;
                case Magenta: printf(M); break;
                case Cyan:    printf(C); break;
                case White:   printf(W); break;
                case Black:   printf(K); break;
                case Space:   
                default:      printf(_); break;
            }
        }
        printf("\n");
    }
}
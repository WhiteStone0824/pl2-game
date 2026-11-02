/* main.c */

#include <sys/ioctl.h>
#include <unistd.h>
#include "view.h"

int main(void) {
    // 表示用のバッファ
    Color screen_buffer[SCREEN_BUFFER_SIZE_WIDTH * SCREEN_BUFFER_SIZE_HEIGHT];

    struct winsize w;
    int term_width;
    int term_height;
    /* ターミナルの画面サイズを取得 */
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl 取得エラー");
        return 1;
    }

    term_width = w.ws_col;
    term_height = w.ws_row;

    /* 以上かどうかを判定 */
    if (term_width < SCREEN_BUFFER_SIZE_WIDTH*2 || term_height < SCREEN_BUFFER_SIZE_HEIGHT) {
        printf("エラー: ターミナルのサイズが小さすぎます。\n");
        printf("現在: %dx%d / 必要: %dx%d 以上\n", term_width, term_height,SCREEN_BUFFER_SIZE_WIDTH*2,SCREEN_BUFFER_SIZE_HEIGHT);
        printf("ターミナルのウィンドウを大きくしてから再度実行してください。\n");
        return 1;
    }

    // テスト用のピクセルアート（すべて赤色で塗りつぶした16x16のオブジェクト）
    PixelArt testArt;
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            testArt.map[y][x] = red;
        }
    }

    // ゲームオブジェクトの初期化（画面中央付近に配置）
    GameObject player;
    player.pos.x = 32;
    player.pos.y = 12;
    player.objectView = &testArt;

    // 描画処理
    clear_screen_buffer(screen_buffer);
    draw_to_object(&player, screen_buffer);
    render_screen(screen_buffer);

    return 0;
}
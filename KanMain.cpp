#include "DxLib.h"
#include "Bspline.h" // ファイル名に合わせてね
#include <math.h>    // 距離計算用

// グラフ表示設定
#define GRAPH_X 100
#define GRAPH_Y 100
#define GRAPH_W 600
#define GRAPH_H 400

// 掴める判定の広さ（半径ピクセル）
#define HIT_RADIUS 15

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(1000, 800, 32);
    ChangeWindowMode(TRUE);
    SetBackgroundColor(255, 255, 255);
    if (DxLib_Init() == -1) return -1;
    SetMouseDispFlag(TRUE);

    KanEdge myEdge;
    InitKanEdge(&myEdge);

    int selectedIndex = -1; // -1は「何も掴んでいない」状態
    int isDragging = 0;     // 0:掴んでない, 1:掴んでいる

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        // ------------------------------------------------
        // 1. マウス情報の取得
        // ------------------------------------------------
        int mx, my;
        GetMousePoint(&mx, &my); // マウスの座標
        int mouseInput = GetMouseInput(); // クリック状態

        // ------------------------------------------------
        // 2. マウス操作ロジック
        // ------------------------------------------------

        // 左クリックされている間
        if ((mouseInput & MOUSE_INPUT_LEFT) != 0) {

            // まだ掴んでいなかったら、近くの点を探す
            if (isDragging == 0) {
                for (int i = 0; i < N; i++) {
                    // 点の画面座標を計算
                    double cx = (double)i / (N - 1);
                    double cy = myEdge.controlY[i];
                    int sx = GRAPH_X + (int)(cx * GRAPH_W);
                    int sy = GRAPH_Y + (int)((0.5 - cy) * GRAPH_H);

                    // マウスとの距離を計算 (ピタゴラスの定理)
                    double dist = sqrt(pow(mx - sx, 2) + pow(my - sy, 2));

                    // 近くにあったら「掴んだ！」状態にする
                    if (dist < HIT_RADIUS) {
                        selectedIndex = i;
                        isDragging = 1;
                        break; // 1つ見
                    }
                }
            }

            if (isDragging == 1 && selectedIndex != -1) {

                double newVal = 0.5 - (double)(my - GRAPH_Y) / GRAPH_H;
                myEdge.controlY[selectedIndex] = newVal;
            }
        }
        else {
            isDragging = 0;
        }



        for (int i = 0; i < N; i++) {
            double cx = (double)i / (N - 1);
            double cy = myEdge.controlY[i];

            int sx = GRAPH_X + (int)(cx * GRAPH_W);
            int sy = GRAPH_Y + (int)((0.5 - cy) * GRAPH_H);

            int color = GetColor(0, 0, 255);
            int size = 5;

            // 選択中、またはマウスが近くにあるときは赤くする（ホバーエフェクト）
            if (i == selectedIndex) {
                color = GetColor(255, 0, 0);
                size = 8;
                DrawFormatString(sx - 20, sy - 30, color, "%.3f", cy);
            }

            // ドラッグ中は線を繋いで視認性を良くする
            if (isDragging == 1 && i == selectedIndex) {
                DrawLine(sx, sy, mx, my, GetColor(255, 200, 200), 1);
            }

            DrawCircle(sx, sy, size, color, TRUE);
        }

        // KAN曲線の描画
        int prevX = -1, prevY = -1;
        for (int i = 0; i <= 200; i++) {
            double x_in = (double)i / 200.0;
            double y_out = Forward(&myEdge, x_in);

            int sx = GRAPH_X + (int)(x_in * GRAPH_W);
            int sy = GRAPH_Y + (int)((0.5 - y_out) * GRAPH_H);

            if (i > 0) {
                DrawLine(prevX, prevY, sx, sy, GetColor(255, 0, 0), 2);
            }
            prevX = sx;
            prevY = sy;
        }

        // 説明文
        DrawString(20, 20, "MOUSE DRAG to Control Points", GetColor(0, 0, 0));

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
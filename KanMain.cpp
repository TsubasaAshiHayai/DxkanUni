#include "DxLib.h"
#include "BSpline.h"

// ÉOÉâÉtï\é¶ê›íË
#define GRAPH_X 100
#define GRAPH_Y 100
#define GRAPH_W 600
#define GRAPH_H 400

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetGraphMode(1000, 800, 32);
    ChangeWindowMode(TRUE);
    SetBackgroundColor(255, 255, 255);
    if (DxLib_Init() == -1) return -1;
    SetMouseDispFlag(TRUE);

    KanEdge myEdge;
    InitKanEdge(&myEdge);

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();


        for (int i = 0; i < N; i++) {
            double cx = (double)i / (N - 1);
            double cy = myEdge.controlY[i]; 

            int sx = GRAPH_X + (int)(cx * GRAPH_W);
            int sy = GRAPH_Y + (int)((0.5 - cy) * GRAPH_H);

            DrawCircle(sx, sy, 5, GetColor(0, 0, 255), TRUE);
            DrawFormatString(sx, sy, GetColor(0, 0, 0), "P%d", i);
        }

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

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
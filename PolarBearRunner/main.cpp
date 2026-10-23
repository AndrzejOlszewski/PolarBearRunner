#include <raylib.h>
#include <iostream>

int main() {
    InitWindow(1200, 720, "PolarRunner");
    SetTargetFPS(60);

    // Cleaned up path based on your exact screenshot
    Texture2D iceBlock = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/iceblock.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(iceBlock, 100, 100, WHITE);


        for (int i = 0; i < 6; i++)
        {
            int xPostion = 500 + (i * iceBlock.width);
            int yPosition = 500;

            DrawTexture(iceBlock, xPostion, yPosition, WHITE);

        }

        EndDrawing();

    }

    UnloadTexture(iceBlock);
    CloseWindow();
    return 0;
}
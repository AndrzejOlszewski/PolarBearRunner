#include <raylib.h>
#include <iostream>

class polarBear
{
public:
    void spawnPolarBear() {

        Texture2D polarBear = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBear.png");
   }
};


int main() {
    InitWindow(1200, 720, "PolarRunner");
    SetTargetFPS(60);

    // Cleaned up path based on your exact screenshot
    Texture2D iceBlock = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/iceblock.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(iceBlock, 100, 100, WHITE);
        DrawTexture(polarBear, 250, 250, WHITE);


        for (int i = 0; i < 6; i++)
        {
            int xPostion = 500 + (i * iceBlock.width);
            int yPosition = 500;

            DrawTexture(iceBlock, xPostion, yPosition, WHITE);

        }

        EndDrawing();

    }

    polarBear PolarBear;

    UnloadTexture(iceBlock);
    UnloadTexture(polarBear);
    CloseWindow();
    return 0;
}
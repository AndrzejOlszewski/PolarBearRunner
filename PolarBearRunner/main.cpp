#include <raylib.h>
#include <iostream>

class PolarBear
{
public:
    Texture2D texture;

    Texture2D idleTexture;
    Texture2D runTexture;

    float x = 150.0f;
    float y = 0.0f;
    float speed = 5.0f;

    int maxFrames = 8;
    int currentFrames = 0;
    float frameTimer = 0.0f;
    float frameSpeed = 0.1f;

    bool isMoving = false;

    void spawnPolarBear(float floorY) {

        idleTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBear.png");
        runTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBearMovement.png");
        texture = idleTexture;




        y = 333.0f;

    }

    void HandleMovement() {
        isMoving = true;
        texture = runTexture;

        frameTimer += GetFrameTime();

        if (frameTimer >= frameSpeed) {
            frameTimer = 0.0f;
            currentFrames++;
            if (currentFrames >= maxFrames) currentFrames = 0;
        }
    }

    void Draw() {
        if (isMoving) {
            float frameWidth = (float)runTexture.width / maxFrames;
            Rectangle sourceRec = { (float)currentFrames * frameWidth, 0.0f, frameWidth, (float)runTexture.height };
            Vector2 position = { x, y };

            DrawTextureRec(runTexture, sourceRec, position, WHITE);
        }
        else {
            DrawTexture(idleTexture, (int)x, (int)y, WHITE);
        }
    }

    void Unload() {
        UnloadTexture(idleTexture);
        UnloadTexture(runTexture);
    }
};

int main() {
    InitWindow(1200, 720, "PolarRunner");
    SetTargetFPS(60);

    Texture2D iceBlock = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/iceblock.png");

    float floorY = 360.0f;

    PolarBear player;
    player.spawnPolarBear(floorY);

    float floorScroll = 0.0f;
    float gameSpeed = 4.0f;

    while (!WindowShouldClose()) {
        player.HandleMovement();

        floorScroll -= gameSpeed;
        if (floorScroll <= -iceBlock.width)
        {
            floorScroll = 0;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(iceBlock, 100, 100, WHITE);

        

        int blocksNeeded = (1200 / iceBlock.width) + 2;
        for (int i = 0; i < blocksNeeded; i++)
        {
            float xPostion = floorScroll + (i * iceBlock.width);
            DrawTexture(iceBlock, (int)xPostion, (int)floorY, WHITE);
        }

        player.Draw();

        EndDrawing();
    }

    // Properly cleaning up resources before the program exits
    player.Unload();
    UnloadTexture(iceBlock);
    CloseWindow();

    return 0;
}
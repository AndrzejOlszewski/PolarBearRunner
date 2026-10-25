#include <raylib.h>
#include <iostream>

class PolarBear
{
public:
    Texture2D texture;

    Texture2D idleTexture;
    Texture2D runTexture;

    float x = 250.0f;
    float y = 250.0f;
    float speed = 5.0f;

    int maxFrames = 8;
    int currentFrames = 0;
    float frameTimer = 0.0f;
    float frameSpeed = 0.1f;
    bool isMoving = false; // FIXED: Removed the duplicate declaration line here

    void spawnPolarBear() {
        idleTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBear.png");
        runTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBearMovement.png");
        texture = idleTexture; // Start idle
    }

    void HandleMovement() {
        isMoving = false;

        // Check for keys and flag movement
        if (IsKeyDown(KEY_W)) { y = y - speed; isMoving = true; }
        if (IsKeyDown(KEY_S)) { y = y + speed; isMoving = true; }
        if (IsKeyDown(KEY_A)) { x = x - speed; isMoving = true; }
        if (IsKeyDown(KEY_D)) { x = x + speed; isMoving = true; }

        if (isMoving) {
            texture = runTexture;

            // FIXED: Added delta time to the timer correctly using +=
            frameTimer += GetFrameTime();

            if (frameTimer >= frameSpeed) {
                frameTimer = 0.0f;
                currentFrames++;
                // FIXED: Changed currentFrame to currentFrames to match your variable name
                if (currentFrames >= maxFrames) currentFrames = 0;
            }
        }
        else {
            texture = idleTexture;
            currentFrames = 0;
        }
    }

    void Draw() {
        if (isMoving) {
            // FIXED: Added a source rectangle to cut out exactly 1 bear from the sheet
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

    PolarBear player;
    player.spawnPolarBear();

    float floorScroll = 0.0f;
    float gameSpeed = 4.0f;

    while (!WindowShouldClose()) {
        player.HandleMovement();

        // 1. Move left and reset when a full block passes the screen edge
        floorScroll -= gameSpeed;
        if (floorScroll <= -iceBlock.width)
        {
            floorScroll = 0;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(iceBlock, 100, 100, WHITE);

        player.Draw();

        int blocksNeeded = (1200 / iceBlock.width) + 2;
        for (int i = 0; i < blocksNeeded; i++)
        {
            // 2. Add floorScroll here so the entire row moves dynamically
            float xPostion = floorScroll + (i * iceBlock.width);
            int yPosition = 500;
            DrawTexture(iceBlock, (int)xPostion, yPosition, WHITE);
        }

        EndDrawing();
    }
}

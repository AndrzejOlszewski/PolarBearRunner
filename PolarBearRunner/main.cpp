#include <raylib.h>
#include <iostream>

class Penguin
{
public:
    Texture2D texture;
    float y;
    float x;

    float speed;
    bool active;

    void spawnPenguin(Texture2D sharedTexture, float startX, float groundY) {
        texture = sharedTexture;
        x = startX;
        y = groundY - texture.height;
        speed = 5.0f;
        active = true;
    }

    void Update() {
        x -= speed;
        // If the penguin rolls off the left side of the screen, recycle it to the right
        if (x + texture.width < 0)
        {
            x = 1200.0f + (GetRandomValue(100, 600)); // Teleports back to the right with random variance
        }
    }

    void Draw() {
        if (active)
        {
            DrawTexture(texture, (int)x, (int)y, WHITE);
        }
    }
};

class PolarBear
{
public:
    Texture2D texture;
    Texture2D idleTexture;
    Texture2D runTexture;

    float x = 150.0f;
    float y = 0.0f;
    float speed = 5.0f;

    // Physics & Jump Variables
    float speedY = 0.0f;
    float gravity = 0.8f;
    float jumpForce = -15.0f;
    float groundY = 0.0f;
    bool isGrounded = true;

    int maxFrames = 8;
    int currentFrames = 0;
    float frameTimer = 0.0f;
    float frameSpeed = 0.1f;
    bool isMoving = false;

    void spawnPolarBear(float floorY) {
        idleTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBear.png");
        runTexture = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PolarBearMovement.png");
        texture = idleTexture;

        // Position the bear cleanly above the middle track line
        y = 333.0f;
        groundY = y;
    }

    void HandleMovement() {
        isMoving = true;
        texture = runTexture;

        // Check for jump input
        if (IsKeyPressed(KEY_SPACE) && isGrounded)
        {
            speedY = jumpForce;
            isGrounded = false;
        }

        // Apply physics update if bear is in the air
        if (!isGrounded)
        {
            speedY += gravity; // Gravity increases downward velocity
            y += speedY;       // FIXED: Moved the bear using speedY instead of static gravity
        }

        // Check if hitting or passing the floor boundary
        if (y >= groundY)
        {
            y = groundY;
            speedY = 0.0f;
            isGrounded = true; // FIXED: Semicolon added here
        }

        // Handle animation frame steps
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
    Texture2D penguinTex = LoadTexture("C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PenguinObstacle.png");

    // Ice blocks placed exactly in the middle of the screen
    float floorY = 360.0f;

    // Fixed array of 3 penguins spaced out down the line
    const int MAX_PENGUINS = 3;
    Penguin obstacles[MAX_PENGUINS];

    // --- INITIAL RESOURCING & ALIGNMENT ---
    for (int i = 0; i < MAX_PENGUINS; i++) {
        // Space them out initially beyond the right side of the screen
        float startX = 1200.0f + (i * 450.0f);
        obstacles[i].spawnPenguin(penguinTex, startX, floorY);

        // Pin them flat to the ice line manually so they match the bear's feet
        obstacles[i].y = 340.0f;
    }

    PolarBear player;
    player.spawnPolarBear(floorY);

    float floorScroll = 0.0f;
    float gameSpeed = 4.0f;

    while (!WindowShouldClose()) {
        player.HandleMovement();

        // Update all penguin positions (makes them move left across the screen)
        for (int i = 0; i < MAX_PENGUINS; i++) {
            obstacles[i].Update();
        }

        floorScroll -= gameSpeed;
        if (floorScroll <= -iceBlock.width)
        {
            floorScroll = 0;
        }

        BeginDrawing();
        ClearBackground(WHITE);

        // Your static test block up top
        DrawTexture(iceBlock, 100, 100, WHITE);

        // Render the scrolling loop row of ice blocks
        int blocksNeeded = (1200 / iceBlock.width) + 2;
        for (int i = 0; i < blocksNeeded; i++)
        {
            float xPostion = floorScroll + (i * iceBlock.width);
            DrawTexture(iceBlock, (int)xPostion, (int)floorY, WHITE);
        }

        // Render all the penguins from our array cleanly
        for (int i = 0; i < MAX_PENGUINS; i++) {
            obstacles[i].Draw();
        }

        // Render the player cleanly on top of everything
        player.Draw();

        EndDrawing();
    }

    // Properly cleaning up resources before the program exits
    player.Unload();
    UnloadTexture(iceBlock);
    UnloadTexture(penguinTex);
    CloseWindow();

    return 0;
}

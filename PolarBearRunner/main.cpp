
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
        // Calculate Y position based on the 2.0x scale used in Draw()
        y = 372.0f - (texture.height * 2.0f);

        speed = 5.0f;
        active = true;
    }

    void Update() {
        x -= speed;

        // If the penguin rolls off the left side, recycle it to the right
        if (x + (texture.width * 2.0f) < 0)
        {
            x = 1200.0f + (GetRandomValue(100, 600));
        }
    }

    void Draw() {
        if (active)
        {
            Rectangle source = {
                0.0f, 0.0f,
                (float)texture.width,
                (float)texture.height
            };

            float scale = 2.0f;

            Rectangle dest = {
                x, y,
                (float)texture.width * scale,
                (float)texture.height * scale
            };

            Vector2 origin = { 0.0f, 0.0f };

            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
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

        // Position the bear above the middle track line
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

        // Apply physics while the bear is in the air
        if (!isGrounded)
        {
            speedY += gravity;
            y += speedY;
        }

        // Check if the bear has reached the floor
        if (y >= groundY)
        {
            y = groundY;
            speedY = 0.0f;
            isGrounded = true;
        }

        // Handle animation frame steps
        frameTimer += GetFrameTime();

        if (frameTimer >= frameSpeed) {
            frameTimer = 0.0f;
            currentFrames++;

            if (currentFrames >= maxFrames)
                currentFrames = 0;
        }
    }

    void Draw() {
        if (isMoving) {
            float frameWidth = (float)runTexture.width / maxFrames;

            Rectangle sourceRec = {
                (float)currentFrames * frameWidth,
                0.0f,
                frameWidth,
                (float)runTexture.height
            };

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

    Texture2D iceBlock = LoadTexture(
        "C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/iceblock.png"
    );

    Texture2D penguinTex = LoadTexture(
        "C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/PenguinObstacle.png"
    );

    // ADDED: Load the background once before the game loop
    Texture2D background = LoadTexture(
        "C:/Users/olsze/source/repos/PolarBearRunner/PolarBearRunner/Assets/Snowbackground.png"
    );

    // Ice blocks placed in the middle of the screen
    float floorY = 360.0f;

    // Fixed array of 3 penguins
    const int MAX_PENGUINS = 3;
    Penguin obstacles[MAX_PENGUINS];

    for (int i = 0; i < MAX_PENGUINS; i++) {
        float startX = 1200.0f + (i * 450.0f);
        obstacles[i].spawnPenguin(penguinTex, startX, floorY);
    }

    PolarBear player;
    player.spawnPolarBear(floorY);

    float floorScroll = 0.0f;
    float gameSpeed = 4.0f;

    while (!WindowShouldClose()) {
        player.HandleMovement();

        // Update penguin positions
        for (int i = 0; i < MAX_PENGUINS; i++) {
            obstacles[i].Update();
        }

        floorScroll -= gameSpeed;

        if (floorScroll <= -iceBlock.width)
        {
            floorScroll = 0;
        }

        BeginDrawing();

        // ADDED: Clear the screen and draw the background full-screen
        ClearBackground(RAYWHITE);

        Rectangle backgroundSource = {
            0.0f,
            0.0f,
            (float)background.width,
            (float)background.height
        };

        Rectangle backgroundDest = {
            0.0f,
            0.0f,
            1200.0f,
            720.0f
        };

        Vector2 backgroundOrigin = { 0.0f, 0.0f };

        DrawTexturePro(
            background,
            backgroundSource,
            backgroundDest,
            backgroundOrigin,
            0.0f,
            WHITE
        );

        // Your original static test block
        DrawTexture(iceBlock, 100, 100, WHITE);

        // Render the scrolling row of ice blocks
        int blocksNeeded = (1200 / iceBlock.width) + 2;

        for (int i = 0; i < blocksNeeded; i++)
        {
            float xPosition = floorScroll + (i * iceBlock.width);

            DrawTexture(
                iceBlock,
                (int)xPosition,
                (int)floorY,
                WHITE
            );
        }

        // Render penguins
        for (int i = 0; i < MAX_PENGUINS; i++) {
            obstacles[i].Draw();
        }

        // Render the player
        player.Draw();

        EndDrawing();
    }

    // Clean up resources
    player.Unload();

    UnloadTexture(iceBlock);
    UnloadTexture(penguinTex);

    // ADDED: Unload the background
    UnloadTexture(background);

    CloseWindow();

    return 0;
}

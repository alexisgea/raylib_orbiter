// To compile use the following command: gcc main.c -lraylib -lopengl32 -lgdi32 -lwinmm -o orbiter.exe

#include "raylib.h"
#include "camera.c"


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "orbiter");
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second


    Vector3 focusPosition = { 0.0f, 0.0f, 0.0f };
    Camera3D camera = InitCamera(focusPosition);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())       // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCameraCustom(&camera);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(focusPosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(focusPosition, 2.0f, 2.0f, 2.0f, MAROON);
                DrawGrid(10, 1.0f);

            EndMode3D();

            // debut texts
            // DrawText("EARTH ORBITING AROUND THE SUN!", 10, 10, 10, DARKGRAY);
            
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
// To compile use the following command: gcc main.c -lraylib -lopengl32 -lgdi32 -lwinmm -o orbiter.exe

#include "raylib.h"
#include "camera.c"
#include "skybox.c"

#define GLSL_VERSION 330


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
    Model skybox = InitSkybox(GLSL_VERSION);

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
            ClearBackground(BLACK); // RAYWHITE

            BeginMode3D(camera);

                DrawSkybox(skybox);

                DrawCube(focusPosition, 2.0f, 2.0f, 2.0f, RED);
                // DrawCubeWires(focusPosition, 2.0f, 2.0f, 2.0f, MAROON);
                DrawGrid(10, 1.0f);

            EndMode3D();

            // debut texts
            // DrawText("EARTH ORBITING AROUND THE SUN!", 10, 10, 10, DARKGRAY);

            DrawFPS(10, 10);
            
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);        // Unload skybox model

    CloseWindow();

    return 0;
}
// To compile use the following command: gcc main.c -lraylib -lopengl32 -lgdi32 -lwinmm -o orbiter.exe

#include "raylib.h"
#include "camera.c"
#include "skybox.c"
#include "isphere.c"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define GLSL_VERSION 330

// Method declaration
//--------------------------------------------------------------------------------------
void UpdateWindow(int *screenWidth, int *screenHeight);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT); //  | FLAG_VSYNC_HINT     FLAG_WINDOW_ALWAYS_RUN
    InitWindow(screenWidth, screenHeight, "orbiter");
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    Vector3 focusPosition = { 0.0f, 0.0f, 0.0f };
    Camera3D camera = InitCamera(focusPosition);
    Model skybox = InitSkybox(GLSL_VERSION);

    // Load model from a generated mesh
    // Model sphere = LoadModelFromMesh(GenMeshSphere(1.0f, 20, 20));
    Model sphere = LoadModelFromMesh(GenIcosphereMesh(2));

    // create and initialise basic light shader
    Shader lightShader = CreateLightShader(GLSL_VERSION);
    // Assign out lighting shader to model
    sphere.materials[0].shader = lightShader;
    // Create lights
    Light light = CreateLight(LIGHT_POINT, (Vector3){ 10, 2, 10 }, Vector3Zero(), YELLOW, lightShader);
    // light.enabled = true;

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())       // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateWindow(&screenWidth, &screenHeight);
        UpdateCameraCustom(&camera);

        // update light shader
        // TODO is it required if the light doesn't move (only the camera moves?)
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(lightShader, light);


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK); // RAYWHITE

            BeginMode3D(camera);

                DrawSkybox(skybox);

                // Draw Planet
                DrawModel(sphere, Vector3Zero(), 1.0f, WHITE);

                // draw star
                DrawSphereWires(light.position, 0.2f, 8, 8, ColorAlpha(light.color, 0.3f));

                // draw orbit
                // DrawLine3D();

                // draw state vectors
                // DrawLine3D();
                
            EndMode3D();

            // draw imgui ui widgets?

            // debut texts
            // DrawText("EARTH ORBITING AROUND THE SUN!", 10, 10, 10, DARKGRAY);

            DrawFPS(10, 10);
            
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(skybox.materials[0].shader);
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);

    UnloadShader(sphere.materials[0].shader);
    UnloadModel(sphere);
    // UnloadShader(lightShader);

    CloseWindow();

    return 0;
}


void UpdateWindow(int *screenWidth, int *screenHeight) {
    // window resizing
    if (IsWindowResized() && !IsWindowMaximized() && !IsWindowFullscreen())
    {
        *screenWidth = GetScreenWidth();
        *screenHeight = GetScreenHeight();
    }

    // check for alt + enter
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {

        if(IsWindowMaximized()) {
            RestoreWindow();
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
        }
        else {
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            MaximizeWindow();
            
            // see what display we are on right now
 			int display = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            
        }
    }
}
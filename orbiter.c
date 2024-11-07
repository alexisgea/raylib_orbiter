/*******************************************************************************************
*
*
*
********************************************************************************************/

// To compile use the following command: gcc main.c -lraylib -lopengl32 -lgdi32 -lwinmm -o orbiter.exe
#include <raylib.h>
#include "camera.c"
#include "skybox.c"
#include "isphere.c"

// #include "stdlib.h"
// #include "math.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define GLSL_VERSION 330

typedef struct WorldState {
    double inverseScale;
    double scale;
    Vector3 center;
} WorldState;

// Method declaration
//--------------------------------------------------------------------------------------
void UpdateWindow(int *screenWidth, int *screenHeight);

const bool USE_SCALE_ZOOM = true;
const float SCALE_ZOOM_SPEED = 1.0f;
const float SCALE_ZOOM_LERP_SPEED = 1.0f;
const double MINIMUM_SCALE = 0.001;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT); //  | FLAG_VSYNC_HINT     FLAG_WINDOW_ALWAYS_RUN
    InitWindow(screenWidth, screenHeight, "orbiter");
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    WorldState worldState = { .inverseScale = 1.0, .scale = 1.0, .center = Vector3Zero()};
    CameraState camera = CreateCamera();
    Model skybox = InitSkybox(GLSL_VERSION);

    // Load model from a generated mesh
    Model sphere = LoadModelFromMesh(GenMeshSphere(1.0f, 20, 20));
    // Model sphere = LoadModelFromMesh(GenIcosphereMesh(2));

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
        //
        // inputs
        //

        // scale OR distance -> wheel scroll (with a bool to set in a config file)
        // focal length -> mouse wheel click held
        // orbit rotation -> left click held
        // look around -> right click held
        float wheelScroll = GetMouseWheelMove();
        Vector2 leftPressDelta = IsMouseButtonDown(0) ? GetMouseDelta() : Vector2Zero();
        Vector2 rightPressDelta = IsMouseButtonDown(1) ? GetMouseDelta() : Vector2Zero();
        Vector2 wheelPressDelta = IsMouseButtonDown(2) ? GetMouseDelta() : Vector2Zero();
        // float detalTime = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        UpdateWindow(&screenWidth, &screenHeight);


        if(USE_SCALE_ZOOM) {
            // TODO Lerp this
            worldState.inverseScale -= wheelScroll * SCALE_ZOOM_SPEED;
            if(worldState.inverseScale < 1.0) worldState.inverseScale = 1.0;
            worldState.scale = 1.0 / worldState.inverseScale;
        }
        float zoomDelta = USE_SCALE_ZOOM ? 0 : wheelScroll;
        float focalDelta = Vector2Length(wheelPressDelta);
        UpdateCameraCustom(&camera, leftPressDelta, zoomDelta, rightPressDelta, focalDelta);

        // update light shader
        float cameraPos[3] = { camera.current.position.x, camera.current.position.y, camera.current.position.z };
        SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(lightShader, light);


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(BLACK); // RAYWHITE

            BeginMode3D(camera.current);

                DrawSkybox(skybox);

                // Draw Planet
                Vector3 planetScaledPos = Vector3Subtract(Vector3Zero(), worldState.center);
                planetScaledPos = Vector3Scale(planetScaledPos, worldState.scale);
                DrawModel(sphere, planetScaledPos, worldState.scale, WHITE);

                // draw star
                Vector3 starScaledPos = Vector3Subtract(light.position, worldState.center);
                starScaledPos = Vector3Scale(starScaledPos, worldState.scale);
                float starRadius = 0.2f * worldState.scale;
                DrawSphereWires(starScaledPos, starRadius, 8, 8, ColorAlpha(light.color, 0.3f));

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
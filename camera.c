#include "raylib.h"
#include "raymath.h"

Camera3D InitCamera(Vector3);
void UpdateCameraCustom(Camera3D *camera);

struct CameraState {
    Camera3D camera;

    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
};


Camera3D InitCamera(Vector3 focusPosition) {
    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 20.0f, 10.0f, 10.0f }; // Camera position
    camera.target = focusPosition;      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type

    // SetCameraMode(camera, CAMERA_CUSTOM); // Set a free camera mode

    return camera;
}


void UpdateCameraCustom(Camera3D *camera) {
    // TODO smoothing movement with a target coord and smooth movement

    // constants
    const Vector3 VERT_AXIS = (Vector3){0.0f, 1.0f, 0.0f};
    const float ROTATION_SPEED = 0.01f;
    const float ZOOM_SPEED = 1.0f;


    // inputs
    // float detalTime = GetFrameTime();
    float wheelVal = GetMouseWheelMove();
    Vector2 mouseDelta;

    // DrawText(TextFormat("mouse delta: %02.02f", GetMouseDelta().y), 10, 10, 10, DARKGRAY);

    // NOTE
    // I will probably have to project the vector for the Y rotation somehow

    if(IsMouseButtonDown(0)) {
        mouseDelta = GetMouseDelta();
    }
    else {
        mouseDelta = Vector2Zero();
    }


    // TODO This part should replaced by a universal scale eventually
    Vector3 camPos = camera->position;
    Vector3 camFocus = camera->target;
    Vector3 camDir = Vector3Subtract(camPos, camFocus);
    float dist = Vector3Length(camDir);
    float finalDist = dist + wheelVal * ZOOM_SPEED;
    if(finalDist <= 0) finalDist = 0.001; // maybe better to setup a very small value
    camDir = Vector3Normalize(camDir);

    // rotate direction of camera by the mouse move values -> TODO smooth with a target value
    Vector3 rightAxis = Vector3CrossProduct(VERT_AXIS, Vector3Negate(camDir));
    Quaternion quat1 = QuaternionFromAxisAngle(VERT_AXIS, -mouseDelta.x * ROTATION_SPEED);

    // NOTE need to clamp this rotatino so it doesn't flip the vector once vertical
    Quaternion quat2 = QuaternionFromAxisAngle(rightAxis, mouseDelta.y * ROTATION_SPEED);
    Quaternion quat = QuaternionMultiply(quat1, quat2);

    Vector3 rotatedDir = Vector3RotateByQuaternion(camDir, quat);
    Vector3 finalDir = rotatedDir;

    DrawText(TextFormat("cam dir: x:%02.02f y:%02.02f z:%02.02f", rotatedDir.x, rotatedDir.y, rotatedDir.z), 10, 40, 20, WHITE);

    // if we had a close to 180 flip, then we want to cancel it
    Vector3 newRightAxis = Vector3CrossProduct(VERT_AXIS, Vector3Negate(rotatedDir));
    float dotProd = Vector3DotProduct(rightAxis, newRightAxis);
    // note: it would be better to set the new direction to exactly vertical 
    if(dotProd < 0) finalDir = camDir;
    // if(dotProd < 0) {
    //     finalDir = camDir.y < 0 ? Vector3Negate(VERT_AXIS) : VERT_AXIS; // set axis to up or down to avoid flipping
    // }
    // DrawText(TextFormat("dot prod: %02.02f", dotProd), 10, 60, 20, WHITE);

    Vector3 newPos = Vector3Scale(finalDir, finalDist);
    camera->position = Vector3Add(newPos, camFocus);
}

void UpdateUniverseState() {

}
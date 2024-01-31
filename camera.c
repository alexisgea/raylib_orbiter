#include "raylib.h"
#include "raymath.h"

Camera3D InitCamera(Vector3);
void UpdateCameraCustom(Camera3D *camera);


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


    // calculate look direction of camera in world coordiantes
    Vector3 camPos = camera->position;
    Vector3 camFocus = camera->target;
    Vector3 camToFocus = Vector3Subtract(camPos, camFocus);
    Vector3 camDir = Vector3Normalize(camToFocus);

    // rotate direction of camera by the mouse move values -> TODO smooth with a target value
    Vector3 rightAxis = Vector3CrossProduct(VERT_AXIS, Vector3Negate(camDir));
    Quaternion quat1 = QuaternionFromAxisAngle(VERT_AXIS, -mouseDelta.x * ROTATION_SPEED);
    Quaternion quat2 = QuaternionFromAxisAngle(rightAxis, mouseDelta.y * ROTATION_SPEED);
    Quaternion quat = QuaternionMultiply(quat1, quat2);

    Vector3 rotatedDir = Vector3RotateByQuaternion(camDir, quat);

    // TODO This part should replaced by a universal scale eventually
    // calcualte distance to the focus object with the new mouse scroll value -> TODO smooth as well
    float distanceToFocus = Vector3Length(camToFocus) + wheelVal * ZOOM_SPEED;

    // update new camera position
    Vector3 newPos = Vector3Scale(rotatedDir, distanceToFocus);
    camera->position = Vector3Add(newPos, camFocus);
}

void UpdateUniverseState() {

}
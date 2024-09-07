#include <raylib.h>
#include <raymath.h>
#include "mathutils.h"

// constants
const double MINIMUM_DIST = 0.001;
const Vector3 VERT_AXIS = (Vector3){0.0f, 1.0f, 0.0f};
const float ROTATION_SPEED = 0.01f;
const float DISTANCE_ZOOM_SPEED = 1.0f;

typedef struct CameraState {
    Camera3D current;
    Quaternion currentRotation; // last calculated rotation

    // target values towards which current will always update
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
} CameraState;

CameraState CreateCamera();
void UpdateCameraCustom(CameraState *camera, Vector2 rotationDelta, float zoomDelta, Vector2 looAroundDelta, float focalDelta);


CameraState CreateCamera() {
    // Define the camera to look into our 3d world
    CameraState camera = { 0 };

    Camera3D current = { 0 };
    current = current;
    current.position = (Vector3){ 0.0f, 0.0f, 10.0f };   // Camera position
    current.target = Vector3Zero();                      // Camera looking at point
    current.up = Vector3Up();                            // Camera up vector (rotation towards target)
    current.fovy = 45.0f;                                // Camera field-of-view Y
    current.projection = CAMERA_PERSPECTIVE;             // Camera mode type
    // SetCameraMode(camera.current, CAMERA_CUSTOM);               // Set a free camera mode

    camera.current = current;
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
    camera.target = Vector3Zero();
    camera.fovy = 45.0f;      

    return camera;
}

void UpdateCameraCustom(CameraState *camera, Vector2 rotationDelta, float zoomDelta, Vector2 looAroundDelta, float focalDelta) {
    // TODO split distance update from rotation udpate
    // consider storing quaterion of current camera then lerping quaternion this way we don't have to calculate it each time
    
    //
    // udpate the camera values
    //

    // getting base camera values
    Vector3 cameraPos = camera->position;
    Vector3 targetPos = camera->target;
    Vector3 cameraToTarget = Vector3Subtract(cameraPos, targetPos);
    Vector3 cameraDir = Vector3Normalize(cameraToTarget);
    float distanceToTarget = Vector3Length(cameraToTarget);

    // zoom
    float finalDistance = distanceToTarget + zoomDelta * DISTANCE_ZOOM_SPEED;
    if(finalDistance <= 0) finalDistance = MINIMUM_DIST; // maybe better to setup a very small value

    // rotation
    Vector3 rightAxis = Vector3CrossProduct(VERT_AXIS, Vector3Negate(cameraDir));
    Quaternion quat1 = QuaternionFromAxisAngle(VERT_AXIS, -rotationDelta.x * ROTATION_SPEED);
    Quaternion quat2 = QuaternionFromAxisAngle(rightAxis, rotationDelta.y * ROTATION_SPEED);
    Quaternion quat = QuaternionMultiply(quat1, quat2);
    // todo do the quaternion 180 flip check here and modify the quaternion to be correct directly
    // QuaternionFromVector3ToVector3();
    // QuaternionFromEuler();
    // QuaternionFromMatrix();
    // QuaternionToEuler();
    // QuaternionToMatrix();
    // QuaternionTransform();
    // QuaternionToAxisAngle();
    

    Vector3 rotatedDir = Vector3RotateByQuaternion(cameraDir, quat);
    Vector3 finalDir = rotatedDir;

    // if we had a close to 180 flip, then we want to cancel it
    Vector3 newRightAxis = Vector3CrossProduct(VERT_AXIS, Vector3Negate(rotatedDir));
    float dotProd = Vector3DotProduct(rightAxis, newRightAxis);
    // note: it would be better to set the new direction to exactly vertical 
    if(dotProd < 0) finalDir = cameraDir;
    // if(dotProd < 0) {
    //     finalDir = camDir.y < 0 ? Vector3Negate(VERT_AXIS) : VERT_AXIS; // set axis to up or down to avoid flipping
    // }
    // DrawText(TextFormat("dot prod: %02.02f", dotProd), 10, 60, 20, WHITE);

    Vector3 newPos = Vector3Scale(finalDir, finalDistance);
    camera->position = Vector3Add(newPos, targetPos);

    //
    // lerp the current camera towards the values -> would probably be cleaner to maintain quaternion and lerp that
    //
    // lerp distance --> TODO
    // lerp dir
    // set cam based on dist and dir
    Vector3 currentPos = camera->current.position;
    Vector3 newPosition = Vector3Lerp(currentPos, camera->position, 0.05f);
    camera->current.position = Vector3Normalize(newPosition);
}
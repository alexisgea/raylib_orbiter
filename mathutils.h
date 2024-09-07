#include <raylib.h>
#include <raymath.h>

// #define PI_D 3.14159265358979323846d

// Vector with y value of 1.0f
Vector3 Vector3Up(void)
{
    Vector3 result = { 0.0f, 1.0f, 0.0f };

    return result;
}

// Vector with x value of 1.0f
Vector3 Vector3Right(void)
{
    Vector3 result = { 1.0f, 0.0f, 0.0f };

    return result;
}

// Vector with z value of 1.0f
Vector3 Vector3Forward(void)
{
    Vector3 result = { 0.0f, 0.0f, 1.0f };

    return result;
}
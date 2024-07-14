#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector.h"

typedef struct {
    Vec3f position;
    Vec3f front;
    Vec3f up;
    Vec3f right;
    Vec3f world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
} EulerCamera;

#endif // CAMERA_H_

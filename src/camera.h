#ifndef CAMERA_H_
#define CAMERA_H_

#include "matrix.h"
#include "vector.h"

enum CameraDir {
  FORWARD = 0U,
  BACKWARD,
  LEFT,
  RIGHT
};

typedef struct {
    float fov_half_degrees;
    float z_near;
    float z_far;

    Vec3f position;
    Vec3f front;
    Vec3f up;
    Vec3f right;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float aspect;

    float last_cursor_position_x;
    float last_cursor_position_y;

    Mat4x4f view;
    Mat4x4f projection;
} PerspectiveCamera;

void camera_update(PerspectiveCamera* camera);

void camera_move(PerspectiveCamera *camera, enum CameraDir dir);

#endif // CAMERA_H_

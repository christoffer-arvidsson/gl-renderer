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
    Vec3f position;
    Vec3f front;
    Vec3f up;
    Vec3f right;
    Vec3f world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    float last_cursor_position_x;
    float last_cursor_position_y;
} EulerCamera;

void camera_get_view_matrix(EulerCamera* camera, Mat4x4f dest);
void camera_update_vectors(EulerCamera *camera);
EulerCamera camera_create_euler(void);

void camera_move(EulerCamera *camera, enum CameraDir dir);

#endif // CAMERA_H_

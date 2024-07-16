#include "vector.h"
#include "camera.h"
#include "trig.h"
#include <math.h>

#define YAW 45.0f
#define PITCH 45.0f
#define SPEED 2.5f
#define SENSITIVITY 0.1f

void camera_update_vectors(EulerCamera *camera) {
    float yaw = degrees_to_rad(camera->yaw);
    float pitch = degrees_to_rad(camera->pitch);

    float cos_yaw = cosf(yaw);
    float sin_yaw = sinf(yaw);
    float cos_pitch = cosf(pitch);
    float sin_pitch = sinf(pitch);

    camera->front[0] = -sin_yaw * cos_pitch;
    camera->front[1] = -sin_pitch;
    camera->front[2] = -cos_yaw * cos_pitch;
    camera->right[0] = -cos_yaw;
    camera->right[1] = 0.0f;
    camera->right[2] = sin_yaw;

    vec3f_cross(camera->front, camera->right, camera->up);

    vec3f_normalize(camera->front);
    vec3f_normalize(camera->right);
    vec3f_normalize(camera->up);
}

EulerCamera create_euler_camera() {
    EulerCamera camera = {0};
    camera.world_up[1] = 1.0f;

    camera.yaw = YAW;
    camera.pitch = PITCH;
    camera.speed = SPEED;
    camera.sensitivity = SENSITIVITY;

    camera_update_vectors(&camera);

    return camera;
}

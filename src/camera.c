#include "camera.h"
#include "matrix.h"
#include "trig.h"
#include "vector.h"
#include <math.h>

#define YAW 0.0f
#define PITCH 0.0f
#define SPEED 2.5f
#define SENSITIVITY 0.1f

void camera_update_view_matrix(PerspectiveCamera *camera, Mat4x4f dest) {
    Vec3f dir = {0};
    vec3f_add(camera->position, camera->front, dir);
    mat4x4f_lookat(camera->position, dir, camera->up, dest);
}

void camera_update_projection_matrix(PerspectiveCamera *camera, Mat4x4f dest) {
    mat4x4f_projection_perspective(degrees_to_rad(camera->fov_half_degrees),
                                   camera->aspect, camera->z_near, camera->z_far,
                                   dest);
}

void camera_update_vectors(PerspectiveCamera *camera) {
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

void camera_update(PerspectiveCamera *camera) {
    camera_update_vectors(camera);
    camera_update_view_matrix(camera, camera->view);
    camera_update_projection_matrix(camera, camera->projection);
}

void camera_move(PerspectiveCamera *camera, enum CameraDir dir) {
    switch (dir) {
        case FORWARD: {
            vec3f_add(camera->position, camera->front, camera->position);
            break;
        }
        case BACKWARD: {
            vec3f_sub(camera->position, camera->front, camera->position);
            break;
        }
        case RIGHT: {
            vec3f_sub(camera->position, camera->right, camera->position);
            break;
        }
        case LEFT: {
            vec3f_add(camera->position, camera->right, camera->position);
            break;
        }
        default:
            break;
    }
}

#ifndef MATRIX_H_
#define MATRIX_H_

#include "quat.h"
#include "vector.h"
#include "trig.h"
#include <stdio.h>
#include <immintrin.h>
#include <string.h>

typedef float Mat4x4f[4][4];

#define MAT4X4F_IDENTITY_INITIALIZER { \
    {1.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 1.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 1.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 1.0f}  \
}

#define MAT4X4F_ZERO_INITIALIZER { \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}, \
    {0.0f, 0.0f, 0.0f, 0.0f}  \
}

void mat4x4f_print(const Mat4x4f m);

void mat4x4f_set_zeroes(Mat4x4f dest);

void mat4x4f_scale(Mat4x4f m1, const Vec3f scale);

void mat4x4f_mul(const Mat4x4f m1, const Mat4x4f m2, Mat4x4f d);

void mat4x4f_translate(Mat4x4f m, const Vec3f t);

void mat4x4f_rotate(Mat4x4f m, const Vec3f axis, float rad);
void mat4x4f_rotate_q(Mat4x4f m, const Quat rot);

void mat4x4f_projection_scaled_ortho(float left, float right, float bottom,
                                     float top, float z_near, float z_far,
                                     Mat4x4f dest);

void mat4x4f_projection_perspective(float fovy, float aspect, float z_near, float z_far, Mat4x4f dest);

void mat4x4f_lookat(const Vec3f eye, const Vec3f center, const Vec3f up,
                   Mat4x4f dest);

#endif // MATRIX_H_

#ifndef LINALG_H_
#define LINALG_H_

#include "linalg.h"
#include "math.h"
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846264338327950288

// Vector

typedef float Vec2f[2];
typedef float Vec3f[3];
typedef float Vec4f[4];
typedef Vec4f Quat;
typedef float Mat4x4f[4][4];

void vec3f_print(const Vec3f v);
void vec3f_clone(const Vec3f src, Vec3f dest);
void vec3f_add(const Vec3f a, const Vec3f b, Vec3f d);
void vec3f_sub(const Vec3f a, const Vec3f b, Vec3f d);
void vec3f_scale(Vec3f a, float s, Vec3f d);
float vec3f_norm(Vec3f v);
void vec3f_normalize(Vec3f v);
void vec3f_cross(const Vec3f v1, const Vec3f v2, Vec3f dest);
float vec3f_dot(const Vec3f v1, const Vec3f v2);

void vec4f_print(const Vec4f v);
void vec4f_clone(const Vec4f src, Vec4f dest);
float vec4f_norm(Vec4f v);
void vec4f_normalize(Vec4f v);

// Matrix

#define MAT4X4F_IDENTITY_INITIALIZER                                           \
  {                                                                            \
    {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f},                        \
        {0.0f, 0.0f, 1.0f, 0.0f}, {                                            \
      0.0f, 0.0f, 0.0f, 1.0f                                                   \
    }                                                                          \
  }

#define MAT4X4F_ZERO_INITIALIZER                                               \
  {                                                                            \
    {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f},                        \
        {0.0f, 0.0f, 0.0f, 0.0f}, {                                            \
      0.0f, 0.0f, 0.0f, 0.0f                                                   \
    }                                                                          \
  }

void mat4x4f_print(const Mat4x4f m);
void mat4x4f_set_zeroes(Mat4x4f dest);
void mat4x4f_set_identity(Mat4x4f dest);
void mat4x4f_scale(Mat4x4f m1, const Vec3f scale);
void mat4x4f_mul(const Mat4x4f m1, const Mat4x4f m2, Mat4x4f d);
void mat4x4f_translate(Mat4x4f m, const Vec3f t);
void mat4x4f_rotate(Mat4x4f m, const Vec3f axis, float rad);
void mat4x4f_rotate_q(Mat4x4f m, const Quat rot);
void mat4x4f_projection_scaled_ortho(float left, float right, float bottom,
                                     float top, float z_near, float z_far,
                                     Mat4x4f dest);
void mat4x4f_projection_perspective(float fovy, float aspect, float z_near,
                                    float z_far, Mat4x4f dest);
void mat4x4f_lookat(const Vec3f eye, const Vec3f center, const Vec3f up,
                    Mat4x4f dest);

// Quat

float degrees_to_rad(float degrees);
void quat(const Vec3f axis, float rad, Quat dest);
void quat_mul(const Quat q0, const Quat q1, Quat dest);
void quat_inverse(const Quat q, Quat dest);
void quat_rotate(const Quat q, const Vec3f v, Vec3f dest);
void quat_div(const Quat q0, const Quat q1, Quat dest);
void quat_slerp(const Quat q0, const Quat q1, float alpha, Quat dest);

// Mathematical functions
float lerp(float a, float b, float t);
float quadratic_bezier(float a, float b, float c, float t);
float cubic_bezier(float a, float b, float c, float d, float t);

#endif // LINALG_H_

#include "matrix.h"
#include <math.h>
#include "vector.h"
#include <stdio.h>
#include <immintrin.h>

void mat4x4f_print(const Mat4x4f m) {
    for (size_t y = 0; y < 4; ++y) {
        for (size_t x = 0; x < 4; ++x) {
            printf("%.2f\t", m[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

void mat4x4f_set_zeroes(Mat4x4f dest) {
    for (size_t y = 0; y < 4U; ++y) {
        for (size_t x = 0; x < 4U; ++x) {
            dest[y][x] = 0.0f;
        }
    }
}

void mat4x4f_scale(Mat4x4f m1, const Vec3f scale) {
    m1[0][0] *= scale[0];
    m1[1][1] *= scale[1];
    m1[2][2] *= scale[2];
}

void mat4x4f_mul(const Mat4x4f m1, const Mat4x4f m2, Mat4x4f d) {
    __m128 row1 = _mm_load_ps(m2[0]);
    __m128 row2 = _mm_load_ps(m2[1]);
    __m128 row3 = _mm_load_ps(m2[2]);
    __m128 row4 = _mm_load_ps(m2[3]);
    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(m1[i][0]);
        __m128 brod2 = _mm_set1_ps(m1[i][1]);
        __m128 brod3 = _mm_set1_ps(m1[i][2]);
        __m128 brod4 = _mm_set1_ps(m1[i][3]);
        __m128 row = _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));
        _mm_store_ps(d[i], row);
    }
}

void mat4x4f_translate(Mat4x4f m, const Vec3f t) {
    m[0][3] += t[0];
    m[1][3] += t[1];
    m[2][3] += t[2];
}

void mat4x4f_rotate(Mat4x4f m, const Vec3f axis, float rad) {
    Vec3f axis_clone = {axis[0], axis[1], axis[2]};
    vec3f_normalize(axis_clone);
    float i = axis[0] * sinf(rad / 2);
    float j = axis[1] * sinf(rad / 2);
    float k = axis[2] * sinf(rad / 2);
    float r = cosf(rad / 2);

    Mat4x4f rot = MAT4X4F_IDENTITY_INITIALIZER;
    float ii = i*i;
    float jj = j*j;
    float kk = k*k;
    float ij = i*j;
    float jr = j*r;
    float kr = k*r;
    float ik = i*k;
    float jk = j*k;
    float ir = i*r;

    rot[0][0] = 1.0f - 2.0f * (jj + kk);
    rot[0][1] = 2.0f * (ij - kr);
    rot[0][2] = 2.0f * (ik + jr);
    rot[1][0] = 2.0f * (ij + kr);
    rot[1][1] = 1.0f - 2.0f * (ii + kk);
    rot[1][2] = 2.0f * (jk - ir);
    rot[2][0] = 2.0f * (ik - jr);
    rot[2][1] = 2.0f * (jk + ir);
    rot[2][2] = 1.0f - 2.0f * (ii + jj);

    mat4x4f_mul(rot, m, m);
}

void mat4x4f_projection_scaled_ortho(float left, float right, float bottom,
                                     float top, float z_near, float z_far,
                                     Mat4x4f dest) {
    mat4x4f_set_zeroes(dest);
    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = 1.0f / (z_far - z_near);

    dest[0][0] = 2.0f * rl;
    dest[1][1] = 2.0f * tb;
    dest[2][2] = 2.0f * fn;

    dest[0][3] = -(right + left) * rl;
    dest[1][3] = -(top + bottom) * tb;
    dest[2][3] = (z_far + z_near) * fn;
    dest[3][3] = 1.0f;
}

void mat4x4f_projection_perspective(float fovy, float aspect, float z_near, float z_far, Mat4x4f dest) {
    float f = 1.0f / tanf(fovy / 2.0f);
    float z_range = z_near - z_far;

    mat4x4f_set_zeroes(dest);

    dest[0][0] = f / aspect;
    dest[1][1] = f;
    dest[2][2] = (z_near + z_far) / z_range;
    dest[2][3] = (2.0f * z_near * z_far) / z_range;
    dest[3][2] = -1.0f;
    dest[3][3] = 0.0f;
}

void mat4x4f_lookat(const Vec3f eye, const Vec3f center, const Vec3f up,
                   Mat4x4f dest) {
    mat4x4f_set_zeroes(dest);
    Vec3f f, s, u;
    vec3f_sub(center, eye, f);
    vec3f_normalize(f);

    vec3f_cross(f, up, s);
    vec3f_normalize(s);

    vec3f_cross(s, f, u);

    dest[0][0] = s[0];
    dest[0][1] = s[1];
    dest[0][2] = s[2];
    dest[0][3] = -vec3f_dot(s, eye);

    dest[1][0] = u[0];
    dest[1][1] = u[1];
    dest[1][2] = u[2];
    dest[1][3] = -vec3f_dot(u, eye);

    dest[2][0]= -f[0];
    dest[2][1]= -f[1];
    dest[2][2] = -f[2];
    dest[2][3] = vec3f_dot(f, eye);

    dest[3][3] = 1.0f;

}

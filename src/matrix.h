#ifndef MATRIX_H_
#define MATRIX_H_

#include "vector.h"
#include "math_defs.h"
#include <stdio.h>
#include <immintrin.h>
#include <string.h>

typedef struct {
    float data[16];
} Mat4x4f;

void mat4x4f_print(const Mat4x4f* m) {
    for (size_t y = 0; y < 4; ++y) {
        for (size_t x = 0; x < 4; ++x) {
            printf("%.2f\t", m->data[y * 4 + x]);
        }
        printf("\n");
    }
    printf("\n");
}

Mat4x4f mat4x4f_identity() {
    return (Mat4x4f) { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
}

void mat4x4f_init_zeroes(Mat4x4f* dest) {
    for (size_t i = 0; i < 16U; ++i) {
        dest->data[i] = 0.0f;
    }
}

void mat4x4f_scale(Mat4x4f* m1, const Vec3f* scale) {
    m1->data[0] *= scale->x;
    m1->data[5] *= scale->y;
    m1->data[10] *= scale->z;
}

void mat4x4f_mul(const Mat4x4f* m1, const Mat4x4f* m2, Mat4x4f* d) {
    __m128 row1 = _mm_load_ps(&m2->data[0]);
    __m128 row2 = _mm_load_ps(&m2->data[4]);
    __m128 row3 = _mm_load_ps(&m2->data[8]);
    __m128 row4 = _mm_load_ps(&m2->data[12]);
    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(m1->data[4*i + 0]);
        __m128 brod2 = _mm_set1_ps(m1->data[4*i + 1]);
        __m128 brod3 = _mm_set1_ps(m1->data[4*i + 2]);
        __m128 brod4 = _mm_set1_ps(m1->data[4*i + 3]);
        __m128 row = _mm_add_ps(
                    _mm_add_ps(
                        _mm_mul_ps(brod1, row1),
                        _mm_mul_ps(brod2, row2)),
                    _mm_add_ps(
                        _mm_mul_ps(brod3, row3),
                        _mm_mul_ps(brod4, row4)));
        _mm_store_ps(&d->data[4*i], row);
    }
}

void mat4x4f_translate(Mat4x4f* m, const Vec3f* t) {
    m->data[3] += t->x;
    m->data[7] += t->y;
    m->data[11] += t->z;
}

void mat4x4f_rotate(Mat4x4f* m, const Vec3f* axis, float rad) {
    Vec3f axis_clone = vec3f_clone(axis);
    vec3f_normalize(&axis_clone);
    float i = axis->x * sinf(rad / 2);
    float j = axis->y * sinf(rad / 2);
    float k = axis->z * sinf(rad / 2);
    float r = cosf(rad / 2);

    Mat4x4f rot = mat4x4f_identity();
    float ii = i*i;
    float jj = j*j;
    float kk = k*k;
    float ij = i*j;
    float jr = j*r;
    float kr = k*r;
    float ik = i*k;
    float jk = j*k;
    float ir = i*r;

    rot.data[0] = 1.0f - 2.0f * (jj + kk);
    rot.data[1] = 2.0f * (ij - kr);
    rot.data[2] = 2.0f * (ik + jr);
    rot.data[4] = 2.0f * (ij + kr);
    rot.data[5] = 1.0f - 2.0f * (ii + kk);
    rot.data[6] = 2.0f * (jk - ir);
    rot.data[8] = 2.0f * (ik - jr);
    rot.data[9] = 2.0f * (jk + ir);
    rot.data[10] = 1.0f - 2.0f * (ii + jj);

    mat4x4f_mul(&rot, m, m);
}

void mat4x4f_projection_scaled_ortho(float left, float right, float bottom,
                                     float top, float z_near, float z_far,
                                     Mat4x4f *dest) {
    mat4x4f_init_zeroes(dest);
    float rl = 1.0f / (right - left);
    float tb = 1.0f / (top - bottom);
    float fn = 1.0f / (z_far - z_near);

    dest->data[0] = 2.0f * rl;
    dest->data[5] = 2.0f * tb;
    dest->data[10] = 2.0f * fn;

    dest->data[3] = -(right + left) * rl;
    dest->data[7] = -(top + bottom) * tb;
    dest->data[11] = (z_far + z_near) * fn;
    dest->data[15] = 1.0f;
}

void mat4x4f_projection_perspective(float fovy, float aspect, float z_near, float z_far, Mat4x4f* dest) {
    float f = 1.0f / tanf(fovy / 2.0f);
    float z_range = z_near - z_far;

    // Clear the matrix first
    mat4x4f_init_zeroes(dest);

    dest->data[0] = f / aspect;
    dest->data[5] = f;
    dest->data[10] = (z_near + z_far) / z_range;
    dest->data[11] = (2.0f * z_near * z_far) / z_range;
    dest->data[14] = -1.0f;
    dest->data[15] = 0.0f;
}

void mat4x4f_lookat(const Vec3f *eye, const Vec3f* center, const Vec3f* up,
                   Mat4x4f *dest) {
    Vec3f f, s, u;
    vec3f_sub(center, eye, &f);
    vec3f_normalize(&f);

    vec3f_cross(&f, up, &s);
    vec3f_normalize(&s);

    vec3f_cross(&s, &f, &u);

    dest->data[0] = s.x;
    dest->data[1] = s.y;
    dest->data[2] = s.z;
    dest->data[3] = -vec3f_dot(&s, eye);

    dest->data[4] = u.x;
    dest->data[5] = u.y;
    dest->data[6] = u.z;
    dest->data[7] = -vec3f_dot(&u, eye);

    dest->data[8] = -f.x;
    dest->data[9] = -f.y;
    dest->data[10] = -f.z;
    dest->data[11] = vec3f_dot(&f, eye);

    dest->data[12] = 0.0f;
    dest->data[13] = 0.0f;
    dest->data[14] = 0.0f;
    dest->data[15] = 1.0f;

}

#endif // MATRIX_H_

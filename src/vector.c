#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void vec3f_print(const Vec3f v) {
    printf("%.2f\t%.2f\t%.2f\n", v[0], v[1], v[2]); 
}

void vec3f_clone(const Vec3f src, Vec3f dest) {
    memcpy(dest, src, 3 * sizeof(dest[0]));
}

void vec3f_add(const Vec3f a, const Vec3f b, Vec3f d) {
    d[0] = a[0] + b[0];
    d[1] = a[1] + b[1];
    d[2] = a[2] + b[2];
}

void vec3f_sub(const Vec3f a, const Vec3f b, Vec3f d) {
    d[0] = a[0] - b[0];
    d[1] = a[1] - b[1];
    d[2] = a[2] - b[2];
}

void vec3f_scale(Vec3f a, float s, Vec3f d) {
    d[0] = a[0] * s;
    d[1] = a[1] * s;
    d[2] = a[2] * s;
}

float vec3f_norm(Vec3f v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vec3f_normalize(Vec3f v) {
    float norm_r = 1.0f / vec3f_norm(v);
    v[0] *= norm_r;
    v[1] *= norm_r;
    v[2] *= norm_r;
}

void vec3f_cross(const Vec3f v1, const Vec3f v2, Vec3f dest) {
    dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

float vec3f_dot(const Vec3f v1, const Vec3f v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void vec4f_print(const Vec4f v) {
    printf("%.2f\t%.2f\t%.2f\t%.2f\n", v[0], v[1], v[2], v[3]); 
}

void vec4f_clone(const Vec4f src, Vec4f dest) {
    memcpy(dest, src, 4 * sizeof(dest[0]));
}

#ifndef VECTOR_H_
#define VECTOR_H_

#include <math.h>

typedef struct {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec4f;

Vec3f vec3f(float x, float y, float z) {
    return (Vec3f) {x, y, z};
}

Vec3f vec3f_clone(const Vec3f* vec) {
    return (Vec3f) { vec->x, vec->y, vec->z };
}

void vec3f_add(const Vec3f* a, const Vec3f* b, Vec3f* d) {
    d->x = a->x + b->x;
    d->y = a->x + b->y;
    d->z = a->x + b->z;
}

void vec3f_sub(const Vec3f* a, const Vec3f* b, Vec3f* d) {
    d->x = a->x - b->x;
    d->y = a->x - b->y;
    d->z = a->x - b->z;
}

void vec3f_scale(Vec3f* a, float s, Vec3f* d) {
    d->x = a->x * s;
    d->y = a->y * s;
    d->z = a->z * s;
}

float vec3f_norm(Vec3f* v) {
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

void vec3f_normalize(Vec3f* v) {
    float norm_r = 1 / vec3f_norm(v);
    v->x *= norm_r;
    v->y *= norm_r;
    v->z *= norm_r;
}

void vec3f_cross(const Vec3f* v1, const Vec3f* v2, Vec3f* dest) {
    dest->x = v1->y * v2->z - v1->z * v2->y;
    dest->y = v1->z * v2->x - v1->x * v2->z;
    dest->z = v1->x * v2->y - v1->y * v2->x;
}

float vec3f_dot(const Vec3f* v1, const Vec3f* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vec4f vec4f(float x, float y, float z, float w) {
    return (Vec4f) {x, y, z, w};
}

Vec4f vec4f_clone(const Vec4f* vec) {
    return (Vec4f) { vec->x, vec->y, vec->z , vec->w };
}

#endif // VECTOR_H_

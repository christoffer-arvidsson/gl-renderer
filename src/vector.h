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


void vec3f_add(Vec3f* a, Vec3f* b) {
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

void vec3f_noramalize(Vec3f* v) {
    float norm_r = 1 / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x *= norm_r;
    v->y *= norm_r;
    v->z *= norm_r;
}

Vec3f vec3f_clone(const Vec3f* vec) {
    return (Vec3f) { vec->x, vec->y, vec->z };
}

Vec4f vec4f(float x, float y, float z, float w) {
    return (Vec4f) {x, y, z, w};
}

Vec4f vec4f_clone(const Vec4f* vec) {
    return (Vec4f) { vec->x, vec->y, vec->z , vec->w };
}

#endif // VECTOR_H_

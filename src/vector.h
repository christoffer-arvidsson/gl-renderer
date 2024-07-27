#ifndef VECTOR_H_
#define VECTOR_H_

typedef float Vec3f[3];
typedef float Vec4f[4];

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


#endif // VECTOR_H_

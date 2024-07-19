#ifndef LIGHTING_H_
#define LIGHTING_H_

#include "vector.h"

typedef struct {
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    float shininess;
} Material;

typedef struct {
    Vec3f position;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
} Light;

#endif  // LIGHTING_H_

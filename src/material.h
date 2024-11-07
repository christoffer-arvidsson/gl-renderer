#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "linalg.h"

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

#endif // MATERIAL_H_

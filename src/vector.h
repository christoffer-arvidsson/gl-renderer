#ifndef VECTOR_H_
#define VECTOR_H_

#include <GL/gl.h>

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vec3f;

Vec3f clone(const Vec3f* vec) {
    return (Vec3f) {
        vec->x,
        vec->y,
        vec->z
    };
}

#endif // VECTOR_H_

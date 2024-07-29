#ifndef VERTEX_H_
#define VERTEX_H_

#include "vector.h"
#include <stdlib.h>
// #define VERTEX_BUFFER_CAPACITY (145620U * 3U)
#define VERTEX_BUFFER_CAPACITY (100U)

typedef struct {
    Vec3f pos;
    float pad1;
    Vec3f color;
    float pad2;
    Vec3f normal;
    float align1;
} Vertex;

typedef struct {
    Vertex data[VERTEX_BUFFER_CAPACITY];
    size_t count;
} VertexBuffer;

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex);

#endif  // VERTEX_H_

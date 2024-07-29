#ifndef VERTEX_H_
#define VERTEX_H_

#include "arena.h"
#include "vector.h"
#include <stdlib.h>

typedef struct {
    Vec3f pos;
    float pad1;
    Vec3f color;
    float pad2;
    Vec3f normal;
    float align1;
} Vertex;

typedef struct {
    Vertex* data;
    size_t count;
    size_t capacity;
} VertexBuffer;

VertexBuffer vertex_buffer_alloc(Region* allocator, size_t capacity);

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex);

#endif  // VERTEX_H_

#ifndef MESH_H_
#define MESH_H_

#include "vector.h"
#include <assert.h>
#include <string.h>

#define VERTEX_BUFFER_CAPACITY (19480 * 3)

typedef struct {
    Vec3f pos;
    Vec3f color;
    Vec3f normal;
} Vertex;

typedef struct {
    Vertex data[VERTEX_BUFFER_CAPACITY];
    size_t count;
} VertexBuffer;

typedef struct {
    VertexBuffer vertices;
} Mesh;

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex);

#endif  // MESH_H_

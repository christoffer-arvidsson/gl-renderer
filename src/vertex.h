#ifndef VERTEX_H_
#define VERTEX_H_

#include <stdlib.h>

#include "arena.h"
#include "linalg.h"

typedef struct {
    Vec4f pos;
    Vec4f color;
    Vec4f normal;
} Vertex;

typedef struct {
    Vertex *data;
    size_t count;
    size_t capacity;
} VertexBuffer;

VertexBuffer vertex_buffer_alloc(Region *allocator, size_t capacity);

void vertex_buffer_push(VertexBuffer *buf, Vertex *vertex);

void generate_polyline_vertices(VertexBuffer *vb, float t0, float t1,
                                size_t n_samples);
void generate_quadratic_bezier_vertices(VertexBuffer *vb, float a, float b,
                                        float c, float t0, float t1,
                                        size_t n_samples);
void generate_cubic_bezier_vertices(VertexBuffer *vb, float a, float b, float c,
                                    float d, float t0, float t1,
                                    size_t n_samples);

#endif  // VERTEX_H_

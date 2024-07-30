#ifndef SHAPE_H_
#define SHAPE_H_

#include "vertex.h"
#include <stdlib.h>

// Mathematical functions
float lerp(float a, float b, float t);
float quadratic_bezier(float a, float b, float c, float t);
float cubic_bezier(float a, float b, float c, float d, float t);

// Vertex generators
void generate_polyline_vertices(VertexBuffer* vb, float t0, float t1, size_t n_samples);
void generate_quadratic_bezier_vertices(VertexBuffer* vb, float a, float b, float c, float t0, float t1, size_t n_samples);
void generate_cubic_bezier_vertices(VertexBuffer* vb, float a, float b, float c, float d, float t0, float t1, size_t n_samples);

#endif  // SHAPE_H_

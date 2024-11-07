#include "vertex.h"

#include <assert.h>
#include <string.h>

#include "arena.h"

VertexBuffer vertex_buffer_alloc(Region *allocator, size_t capacity) {
    void *data = region_alloc(allocator, sizeof(Vertex) * capacity);
    VertexBuffer vb = {
        .data = (Vertex *)data, .count = 0U, .capacity = capacity};
    return vb;
}

void vertex_buffer_push(VertexBuffer *buf, Vertex *vertex) {
    assert(buf->count < buf->capacity);
    memcpy(&buf->data[buf->count], vertex, sizeof(*vertex));
    buf->count++;
}

void generate_polyline_vertices(VertexBuffer *vb, float t0, float t1,
                                size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = t * t;

        Vertex v = {
            .pos = {t, y, 0.0f, 0.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f},
            .normal = {0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = t * t;
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}

void generate_quadratic_bezier_vertices(VertexBuffer *vb, float a, float b,
                                        float c, float t0, float t1,
                                        size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = quadratic_bezier(a, b, c, t);

        Vertex v = {
            .pos = {t, y, 0.0f, 0.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f},
            .normal = {0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = quadratic_bezier(a, b, c, t);
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}
void generate_cubic_bezier_vertices(VertexBuffer *vb, float a, float b, float c,
                                    float d, float t0, float t1,
                                    size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = cubic_bezier(a, b, c, d, t);

        Vertex v = {
            .pos = {t, y, 0.0f, 0.0f},
            .color = {1.0f, 1.0f, 1.0f, 1.0f},
            .normal = {0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = cubic_bezier(a, b, c, d, t);
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}

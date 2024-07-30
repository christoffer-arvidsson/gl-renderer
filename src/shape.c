#include "shape.h"
#include "vertex.h"

float lerp(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

float quadratic_bezier(float a, float b, float c, float t) {
    float t_ab = lerp(a, b, t);
    float t_bc = lerp(b, c, t);
    float t_abc = lerp(t_ab, t_bc, t);
    return t_abc;
}

float cubic_bezier(float a, float b, float c, float d, float t) {
    float t_ab = lerp(a, b, t);
    float t_bc = lerp(b, c, t);
    float t_cd = lerp(c, d, t);
    float t_abc = lerp(t_ab, t_bc, t);
    float t_bcd = lerp(t_bc, t_cd, t);
    float t_abcd = lerp(t_abc, t_bcd, t);
    return t_abcd;
}

void generate_polyline_vertices(VertexBuffer* vb, float t0, float t1, size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = t*t;

        Vertex v = {
            .pos={t, y, 0.0f, 0.0f},
            .color={1.0f, 1.0f, 1.0f, 1.0f},
            .normal={0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = t*t;
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}

void generate_quadratic_bezier_vertices(VertexBuffer* vb, float a, float b, float c, float t0, float t1, size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = quadratic_bezier(a, b, c, t);

        Vertex v = {
            .pos={t, y, 0.0f, 0.0f},
            .color={1.0f, 1.0f, 1.0f, 1.0f},
            .normal={0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = quadratic_bezier(a, b, c, t);
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}
void generate_cubic_bezier_vertices(VertexBuffer* vb, float a, float b, float c, float d, float t0, float t1, size_t n_samples) {
    float dt = (t1 - t0) / n_samples;
    float t = t0;
    for (size_t i = 0U; i < n_samples; ++i) {
        // y = t*t  todo: replace with 3d line function generator
        float y = cubic_bezier(a, b, c, d, t);

        Vertex v = {
            .pos={t, y, 0.0f, 0.0f},
            .color={1.0f, 1.0f, 1.0f, 1.0f},
            .normal={0.0f, 0.0f, 0.0f, 0.0f},
        };
        vertex_buffer_push(vb, &v);

        t += dt;
        y = cubic_bezier(a, b, c, d, t);
        v.pos[0] = t;
        v.pos[1] = y;
        vertex_buffer_push(vb, &v);
    }
}

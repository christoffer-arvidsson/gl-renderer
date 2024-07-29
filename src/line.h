#ifndef LINE_H
#define LINE_H

#include "camera.h"
#include "shader.h"
#include "vertex.h"

typedef struct {
    VertexBuffer vertices;
    Vec2f resolution;
    float thickness;
    Vec2f aa_radius;

    GLuint vao;
    GLuint ssbo;
    Shader shader;
} LinesRenderer;

void lines_renderer_init(LinesRenderer* renderer);
// void lines_renderer_draw(const LinesRenderer* renderer, const PerspectiveCamera* camera);
void lines_renderer_draw(const LinesRenderer *renderer, const PerspectiveCamera* camera);

#endif  // LINE_H

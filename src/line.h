#ifndef LINE_H
#define LINE_H

#include "camera.h"
#include "shader.h"


typedef struct {
    GLuint vao;
    GLuint vbo;
    Shader shader;
} LinesRenderer;

void lines_renderer_init(LinesRenderer* renderer);
void lines_renderer_draw(const LinesRenderer* renderer, const PerspectiveCamera* camera);

#endif  // LINE_H

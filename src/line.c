#include "line.h"
#include "camera.h"
#include "shader.h"
#include "vector.h"

typedef enum {
    POSITION_ATTRIB = 0,
    COLOR_ATTRIB = 1
} LineAttributes;

typedef struct {
    Vec3f start;
    Vec3f start_color;
    Vec3f end;
    Vec3f end_color;
} Line;

Line AXIS_LINES[3] = {
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
};

void lines_renderer_init(LinesRenderer* renderer) {
    shader_create("assets/shaders/axis.vert", "assets/shaders/axis.frag",
                  &renderer->shader);
    shader_use(&renderer->shader);

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AXIS_LINES), AXIS_LINES,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6 *sizeof(GLfloat),
                          (GLvoid *)offsetof(Line, start));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (GLvoid *)(offsetof(Line, start_color)));

    glUseProgram(0);
    glBindVertexArray(0);

}

void lines_renderer_draw(const LinesRenderer *renderer, const PerspectiveCamera* camera) {
    shader_use(&renderer->shader);
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    shader_set_mat4x4f(&renderer->shader, "view", camera->view);
    shader_set_mat4x4f(&renderer->shader, "projection", camera->projection);

    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

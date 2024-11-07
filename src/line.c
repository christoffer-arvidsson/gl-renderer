#include "line.h"
#include "camera.h"
#include "linalg.h"
#include "shader.h"
#include "vertex.h"
#include <stdio.h>

void lines_renderer_init(LinesRenderer *renderer) {
  shader_create("assets/shaders/line.vert", "assets/shaders/line.frag",
                &renderer->shader);
  shader_use(&renderer->shader);

  glGenVertexArrays(1, &renderer->vao);
  glBindVertexArray(renderer->vao);

  glGenBuffers(1, &renderer->ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer->ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               renderer->vertices.capacity * sizeof(renderer->vertices.data[0]),
               renderer->vertices.data, GL_DYNAMIC_DRAW);
  glUseProgram(0);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  glBindVertexArray(0);
}

void lines_renderer_draw(const LinesRenderer *renderer,
                         const PerspectiveCamera *camera) {
  shader_use(&renderer->shader);
  glBindVertexArray(renderer->vao);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderer->ssbo);

  shader_set_mat4x4f(&renderer->shader, "view", camera->view);
  shader_set_mat4x4f(&renderer->shader, "projection", camera->projection);
  shader_set_vec2f(&renderer->shader, "viewport_size", camera->viewport);
  shader_set_vec2f(&renderer->shader, "aa_radius", renderer->aa_radius);
  shader_set_float(&renderer->shader, "thickness", renderer->thickness);

  glDrawArrays(GL_TRIANGLES, 0U, 3U * renderer->vertices.count);
  glBindVertexArray(0);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
  glUseProgram(0);
}

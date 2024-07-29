#include "mesh.h"
#include "camera.h"
#include "lighting.h"
#include "matrix.h"
#include "shader.h"
#include <GL/gl.h>

void mesh_renderer_init(MeshRenderer* renderer) {
    const VertexBuffer* vertices = &renderer->mesh.vertices;

    shader_create("assets/shaders/main.vert", "assets/shaders/main.frag",
                  &renderer->shader);
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 VERTEX_BUFFER_CAPACITY * sizeof(vertices->data[0]),
                 vertices->data, GL_STATIC_DRAW);

    glVertexAttribPointer(MESH_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices->data[0]), (GLvoid *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(MESH_POSITION_ATTRIB);

    glVertexAttribPointer(MESH_COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices->data[0]), (GLvoid *)offsetof(Vertex, color));
    glEnableVertexAttribArray(MESH_COLOR_ATTRIB);

    glVertexAttribPointer(MESH_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices->data[0]), (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(MESH_NORMAL_ATTRIB);
}

void mesh_renderer_draw(const MeshRenderer* renderer,
                        const PerspectiveCamera* camera,
                        const Mat4x4f model) {
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    const VertexBuffer* vertices = &renderer->mesh.vertices;
    glBufferData(GL_ARRAY_BUFFER,
                 vertices->count * sizeof(vertices->data[0]),
                 (GLvoid *)vertices->data, GL_STATIC_DRAW);

    shader_use(&renderer->shader);

    shader_set_mat4x4f(&renderer->shader, "model", model);
    shader_set_mat4x4f(&renderer->shader, "view", camera->view);
    shader_set_mat4x4f(&renderer->shader, "projection", camera->projection);
    shader_set_vec3f(&renderer->shader, "view_pos", camera->position);

    shader_set_vec3f(&renderer->shader, "material.ambient", renderer->material.ambient);
    shader_set_vec3f(&renderer->shader, "material.specular", renderer->material.specular);
    shader_set_vec3f(&renderer->shader, "material.diffuse", renderer->material.diffuse);
    shader_set_float(&renderer->shader, "material.shininess", renderer->material.shininess);

    shader_set_vec3f(&renderer->shader, "light.ambient", renderer->light.ambient);
    shader_set_vec3f(&renderer->shader, "light.specular", renderer->light.specular);
    shader_set_vec3f(&renderer->shader, "light.diffuse", renderer->light.diffuse);
    shader_set_vec3f(&renderer->shader, "light.position", renderer->light.position);

    glDrawArrays(GL_TRIANGLES, 0, vertices->count);
    glUseProgram(0);
    glBindVertexArray(0);
}

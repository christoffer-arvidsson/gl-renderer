#include "shader.h"
#include "linalg.h"
#include <GL/glew.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *shader_type_as_cstr(GLenum shader_type) {
  switch (shader_type) {
  case GL_VERTEX_SHADER:
    return "GL_VERTEX_SHADER";
  case GL_FRAGMENT_SHADER:
    return "GL_FRAGMENT_SHADER";
  default:
    return "(Unknown)";
  }
}

const char *read_shader_content(const char *file_name) {
  FILE *fp;
  long size = 0;
  char *shader_content;

  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Failed reading shader file: %s", file_name);
    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  size = ftell(fp) + 1;
  fclose(fp);

  fp = fopen(file_name, "r");
  shader_content = (char *)memset(malloc(size), '\0', size);
  fread(shader_content, 1, size - 1, fp);
  fclose(fp);

  return shader_content;
}

bool compile_shader_source(const GLchar *source, GLenum shader_type,
                           GLuint *shader) {
  *shader = glCreateShader(shader_type);
  if (*shader == 0) {
    fprintf(stderr, "Could not create shader: %s\n",
            shader_type_as_cstr(shader_type));
  }
  glShaderSource(*shader, 1, &source, NULL);
  glCompileShader(*shader);

  GLint compiled = 0;
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLchar message[1024];
    GLsizei message_size = 0;
    glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
    fprintf(stderr, "ERROR: shader compilation failed: %s\n",
            shader_type_as_cstr(shader_type));
    fprintf(stderr, "%.*s\n", message_size, message);
    return false;
  }

  return true;
}

bool link_shader_program(GLuint vert_shader, GLuint frag_shader,
                         GLuint *program) {
  *program = glCreateProgram();
  glAttachShader(*program, vert_shader);
  glAttachShader(*program, frag_shader);
  glLinkProgram(*program);

  GLint linked = 0;
  glUseProgram(*program);
  glGetProgramiv(*program, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLchar message[1024];
    GLsizei message_size = 0;
    glGetProgramInfoLog(*program, sizeof(message), &message_size, message);
    fprintf(stderr, "ERROR: failed linking shader program: %.*s\n",
            message_size, message);
    return false;
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return true;
}

void shader_create(const char *vert_path, const char *frag_path,
                   Shader *shader) {
  const char *vertex_shader_source = read_shader_content(vert_path);
  const char *frag_shader_source = read_shader_content(frag_path);

  GLuint vert_shader = 0;
  if (!compile_shader_source(vertex_shader_source, GL_VERTEX_SHADER,
                             &vert_shader)) {
    exit(1);
  }

  GLuint frag_shader = 0;
  if (!compile_shader_source(frag_shader_source, GL_FRAGMENT_SHADER,
                             &frag_shader)) {
    exit(1);
  }

  if (!link_shader_program(vert_shader, frag_shader, &shader->program_id)) {
    exit(1);
  }
}

void shader_delete(Shader *shader) { glDeleteProgram(shader->program_id); }

void shader_use(const Shader *shader) { glUseProgram(shader->program_id); }

void shader_set_mat4x4f(const Shader *shader, char *name, const Mat4x4f mat) {
  GLint uniform = glGetUniformLocation(shader->program_id, name);
  glUniformMatrix4fv(uniform, 1, GL_TRUE, (GLfloat *)mat);
}

void shader_set_vec3f(const Shader *shader, char *name, const Vec3f v) {
  GLint uniform = glGetUniformLocation(shader->program_id, name);
  glUniform3f(uniform, v[0], v[1], v[2]);
}

void shader_set_vec2f(const Shader *shader, char *name, const Vec2f v) {
  GLint uniform = glGetUniformLocation(shader->program_id, name);
  glUniform2f(uniform, v[0], v[1]);
}

void shader_set_float(const Shader *shader, char *name, float v) {
  GLint uniform = glGetUniformLocation(shader->program_id, name);
  glUniform1f(uniform, v);
}

void shader_set_int(const Shader *shader, char *name, int v) {
  GLint uniform = glGetUniformLocation(shader->program_id, name);
  glUniform1i(uniform, v);
}

#ifndef SHADER_H_
#define SHADER_H_

#include "matrix.h"
#include "vector.h"
#include <GL/glew.h>

typedef struct {
    GLuint program_id;
} Shader;

void shader_create(const char *vert_path, const char *frag_path, Shader *shader);
void shader_use(const Shader *shader);
void shader_delete(Shader *shader);

void shader_set_mat4x4f(const Shader* shader, char* name, const Mat4x4f mat);
void shader_set_vec3f(const Shader* shader, char* name, const Vec3f v);
void shader_set_vec2f(const Shader* shader, char* name, const Vec2f v);
void shader_set_float(const Shader* shader, char* name, float v);
void shader_set_int(const Shader* shader, char* name, int v);

#endif  // SHADER_H_

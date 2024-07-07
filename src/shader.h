#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef struct {
    GLuint program_id;
} Shader;

void shader_create(const char *vert_path, const char *frag_path, Shader *shader);
void shader_use(const Shader *shader);
void shader_delete(Shader *shader);

#endif /* SHADER_H */

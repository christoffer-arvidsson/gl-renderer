#ifndef MESH_H_
#define MESH_H_

#include "camera.h"
#include "lighting.h"
#include "shader.h"
#include "vector.h"
#include "vertex.h"
#include <assert.h>
#include <string.h>

#include <GL/gl.h>

typedef enum {
    MESH_POSITION_ATTRIB = 0,
    MESH_COLOR_ATTRIB = 1,
    MESH_NORMAL_ATTRIB = 2
} MeshAttributes;

typedef struct {
    VertexBuffer vertices;
} Mesh;

typedef struct {
    Material material;
    Light light;
    Mesh mesh;

    GLuint vao;
    GLuint vbo;
    Shader shader;
} MeshRenderer;

void mesh_renderer_init(MeshRenderer* renderer);
void mesh_renderer_draw(const MeshRenderer* renderer,
                        const PerspectiveCamera* camera,
                        const Mat4x4f model);


#endif  // MESH_H_

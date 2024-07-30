#ifndef MESH_H_
#define MESH_H_

#include "arena.h"
#include "camera.h"
#include "lighting.h"
#include "matrix.h"
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

    Mat4x4f model;

    GLuint vao;
    GLuint vbo;
    Shader shader;
} MeshRenderer;

Mesh mesh_alloc(Region* allocator, size_t vertex_capacity);
void mesh_renderer_init(MeshRenderer* renderer);
void mesh_renderer_draw(const MeshRenderer* renderer,
                        const PerspectiveCamera* camera);


#endif  // MESH_H_

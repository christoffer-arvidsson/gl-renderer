#ifndef MESH_H_
#define MESH_H_

#include "camera.h"
#include "lighting.h"
#include "shader.h"
#include "vector.h"
#include <assert.h>
#include <string.h>

#include <GL/gl.h>

#define VERTEX_BUFFER_CAPACITY (145620U * 3U)

typedef enum {
    POSITION_ATTRIB = 0,
    COLOR_ATTRIB = 1,
    NORMAL_ATTRIB = 2
} MeshAttributes;

typedef struct {
    Vec3f pos;
    Vec3f color;
    Vec3f normal;
} Vertex;

typedef struct {
    Vertex data[VERTEX_BUFFER_CAPACITY];
    size_t count;
} VertexBuffer;

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

void vertex_buffer_push(VertexBuffer* buf, Vertex* vertex);

void mesh_renderer_init(MeshRenderer* renderer);
void mesh_renderer_draw(const MeshRenderer* renderer,
                        const PerspectiveCamera* camera,
                        const Mat4x4f model);


#endif  // MESH_H_

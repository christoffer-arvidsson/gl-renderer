
#include "shader.h"
#include <assert.h>
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 800U
#define SCREEN_HEIGHT 450U
#define BUFFER_CAPACITY 128U
#define NUM_TRIANGLES 1

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                      GLsizei length, const GLchar *message,
                      const void *userParam) {
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
            message);
}

GLFWwindow *create_window() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window =
        glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "opengl", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit()) {
        fprintf(stderr, "ERROR: Could not initialize GLEW!\n");
        exit(1);
    }

    if (!GLEW_EXT_draw_instanced) {
        fprintf(stderr, "ERROR: Support for EXT_draw_instanced is required!\n");
        exit(1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glUseProgram(program);

    return window;
}

typedef enum {
    POSITION_ATTRIB = 0,
    COLOR_ATTRIB,
} Attribs;

typedef struct {
    GLfloat data[BUFFER_CAPACITY];
    size_t count;
} Buffer;

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    Shader shader;
} Renderer;

typedef Vec3f Vertex;

typedef struct {
    Vertex data[BUFFER_CAPACITY];
    size_t count;
} TriangleBuffer;

void triangle_buffer_push(TriangleBuffer* buf, Vertex vertex) {
    assert(buf->count < BUFFER_CAPACITY);
    memcpy(&buf->data[buf->count], &vertex, sizeof(vertex));
    buf->count++;
}

void renderer_init_buffers(Renderer *renderer,
                           const TriangleBuffer *triangle_buffer,
                           const Buffer *indices_buffer) {
    shader_create("assets/shaders/main.vert", "assets/shaders/main.frag",
                  &renderer->shader);
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, BUFFER_CAPACITY * sizeof(indices_buffer->data[0]), indices_buffer->data, GL_STATIC_DRAW);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, BUFFER_CAPACITY * sizeof(triangle_buffer->data[0]), triangle_buffer->data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(POSITION_ATTRIB);
    glVertexAttribPointer(POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(triangle_buffer->data[0]), (GLvoid *)0);
    // glVertexAttribDivisor(POSITION_ATTRIB, 1);
    //
    // glEnableVertexAttribArray(COLOR_ATTRIB);
    // glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE,
    //                       sizeof(vertex_buffer->data[0]),
    //
    //                       (GLvoid *)(sizeof(GLfloat) * 2));
    // glVertexAttribDivisor(COLOR_ATTRIB, 1);
    //
}

void renderer_draw_triangles(const Renderer *renderer,
                             const TriangleBuffer *triangle_buffer) {
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    glBufferData(GL_ARRAY_BUFFER,
                    triangle_buffer->count * sizeof(triangle_buffer->data[0]),
                    (GLvoid *)triangle_buffer->data, GL_STATIC_DRAW);

    shader_use(&renderer->shader);

    glDrawArrays(GL_TRIANGLES, 0, triangle_buffer->count);
    glUseProgram(0);
    glBindVertexArray(0);
}

int main() {
    GLFWwindow *window = create_window();

    Renderer renderer = {0};

    TriangleBuffer triangle_buffer = {0};
    triangle_buffer_push(&triangle_buffer, (Vertex) {-0.5f, -0.5f, 0.0f});
    triangle_buffer_push(&triangle_buffer, (Vertex) {0.5f, -0.5f, 0.0f});
    triangle_buffer_push(&triangle_buffer, (Vertex) {0.0f, 0.5f, 0.0f});

    Buffer indices_buffer = { { 1, 1, 1, 2, 2, 2 }, 6};

    renderer_init_buffers(&renderer, &triangle_buffer, &indices_buffer);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer_draw_triangles(&renderer, &triangle_buffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
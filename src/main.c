
#include "camera.h"
#include "matrix.h"
#include "mesh.h"
#include "quat.h"
#include "shader.h"
#include "stdbool.h"
#include "stddef.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utah.h"

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 1920U
#define SCREEN_HEIGHT 1080U

typedef enum {
    POSITION_ATTRIB = 0,
    COLOR_ATTRIB = 1,
    NORMAL_ATTRIB = 2
} Attribs;

typedef struct {
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    float shininess;
} Material;

typedef struct {
    Vec3f position;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
} Light;

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

void camera_mouse_callback(GLFWwindow *window, double x_offset,
                           double y_offset) {
    PerspectiveCamera *camera = glfwGetWindowUserPointer(window);
    float x_off = camera->last_cursor_position_x - (float)x_offset;
    float y_off = (float)y_offset - camera->last_cursor_position_y;
    camera->last_cursor_position_x = x_offset;
    camera->last_cursor_position_y = y_offset;
    x_off *= camera->sensitivity;
    y_off *= camera->sensitivity;
    camera->yaw += x_off;
    camera->pitch += y_off;

    camera->pitch = fmax(fmin(camera->pitch, 89.0f), -89.0f);

    camera_update(camera);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    (void) mods; (void) scancode;
    PerspectiveCamera *camera = glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_W) &&
        (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        camera_move(camera, LEFT);
    } else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_B) &&
        (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        camera_move(camera, RIGHT);
    } else if ((key == GLFW_KEY_UP || key == GLFW_KEY_F) &&
        (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        camera_move(camera, FORWARD);
    } else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_P) &&
        (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        camera_move(camera, BACKWARD);
    }
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
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, 0);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera_mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

typedef struct {
    GLfloat data[VERTEX_BUFFER_CAPACITY];
    size_t count;
} Buffer;

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    Shader shader;
} Renderer;

void renderer_init_buffers(Renderer *renderer,
                           const VertexBuffer *vertex_buffer,
                           const Buffer *indices_buffer) {
    shader_create("assets/shaders/main.vert", "assets/shaders/main.frag",
                  &renderer->shader);
    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 VERTEX_BUFFER_CAPACITY * sizeof(indices_buffer->data[0]),
                 indices_buffer->data, GL_STATIC_DRAW);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 VERTEX_BUFFER_CAPACITY * sizeof(vertex_buffer->data[0]),
                 vertex_buffer->data, GL_STATIC_DRAW);

    glVertexAttribPointer(POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_buffer->data[0]), (GLvoid *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(POSITION_ATTRIB);

    glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_buffer->data[0]), (GLvoid *)offsetof(Vertex, color));
    glEnableVertexAttribArray(COLOR_ATTRIB);

    glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_buffer->data[0]), (GLvoid *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(NORMAL_ATTRIB);
}

void renderer_draw_triangles(const Renderer *renderer, const Mat4x4f model,
                             const Mat4x4f view, const Mat4x4f projection,
                             const Vec3f view_pos, const VertexBuffer *vertex_buffer) {
    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 vertex_buffer->count * sizeof(vertex_buffer->data[0]),
                 (GLvoid *)vertex_buffer->data, GL_STATIC_DRAW);

    shader_use(&renderer->shader);

    GLint model_uniform =
        glGetUniformLocation(renderer->shader.program_id, "model");
    glUniformMatrix4fv(model_uniform, 1, GL_TRUE, (GLfloat *)model);

    GLint view_uniform =
        glGetUniformLocation(renderer->shader.program_id, "view");
    glUniformMatrix4fv(view_uniform, 1, GL_TRUE, (GLfloat *)view);

    GLint proj_uniform =
        glGetUniformLocation(renderer->shader.program_id, "projection");
    glUniformMatrix4fv(proj_uniform, 1, GL_TRUE, (GLfloat *)projection);

    GLint view_pos_uniform = glGetUniformLocation(renderer->shader.program_id, "view_pos");
    glUniform3f(view_pos_uniform, view_pos[0], view_pos[1], view_pos[2]);

    Material material = {
        .ambient = {1.0f, 0.5f, 0.31f},
        .specular = {1.0f, 0.5f, 0.31f},
        .diffuse = {0.5f, 0.5f, 0.5f},
        .shininess = 32.0f
    };
    Light light = {
        .position = {25.0f, 25.0f, 25.0f},
        .ambient = {0.2f, 0.2f, 0.2f},
        .specular = {0.5f, 0.5f, 0.5f},
        .diffuse = {1.0f, 1.0f, 1.0f}
    };
    GLint mat_ambient_uniform = glGetUniformLocation(renderer->shader.program_id, "material.ambient");
    glUniform3f(mat_ambient_uniform, material.ambient[0], material.ambient[1], material.ambient[2]);
    GLint mat_specular_uniform = glGetUniformLocation(renderer->shader.program_id, "material.specular");
    glUniform3f(mat_specular_uniform, material.ambient[0], material.ambient[1], material.ambient[2]);
    GLint mat_diffuse_uniform = glGetUniformLocation(renderer->shader.program_id, "material.diffuse");
    glUniform3f(mat_diffuse_uniform, material.ambient[0], material.ambient[1], material.ambient[2]);
    GLint mat_shininess_uniform = glGetUniformLocation(renderer->shader.program_id, "material.shininess");
    glUniform1f(mat_shininess_uniform, material.shininess);

    GLint light_position_uniform = glGetUniformLocation(renderer->shader.program_id, "light.position");
    glUniform3f(light_position_uniform, light.position[0], light.position[1], light.position[2]);
    GLint light_ambient_uniform = glGetUniformLocation(renderer->shader.program_id, "light.ambient");
    glUniform3f(light_ambient_uniform, light.ambient[0], light.ambient[1], light.ambient[2]);
    GLint light_specular_uniform = glGetUniformLocation(renderer->shader.program_id, "light.specular");
    glUniform3f(light_specular_uniform, light.ambient[0], light.ambient[1], light.ambient[2]);
    GLint light_diffuse_uniform = glGetUniformLocation(renderer->shader.program_id, "light.diffuse");
    glUniform3f(light_diffuse_uniform, light.ambient[0], light.ambient[1], light.ambient[2]);

    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer->count);
    glUseProgram(0);
    glBindVertexArray(0);
}

Vertex basic_vertex(float x, float y, float z) {
    return (Vertex) {
        .pos = {x, y, z},
        .color = {1.0f, 1.0f, 1.0f}
    };
}

int main() {
    GLFWwindow *window = create_window();

    Renderer renderer = {0};

    Mesh mesh = {0};

    size_t num_triangles = 0;
    Triangle* triangles = load_teapot_vertices("assets/meshes/teapot.txt", &num_triangles);
    for (size_t i = 0U; i < num_triangles; ++i) {
        triangles[i].v1.color[0] = 1.0f;
        triangles[i].v2.color[0] = 1.0f;
        triangles[i].v3.color[0] = 1.0f;

        triangles[i].v1.color[1] = 1.0f;
        triangles[i].v2.color[1] = 1.0f;
        triangles[i].v3.color[1] = 1.0f;

        triangles[i].v1.color[2] = 1.0f;
        triangles[i].v2.color[2] = 1.0f;
        triangles[i].v3.color[2] = 1.0f;
        vertex_buffer_push(&mesh.vertices, &triangles[i].v1);
        vertex_buffer_push(&mesh.vertices, &triangles[i].v2);
        vertex_buffer_push(&mesh.vertices, &triangles[i].v3);
    }
    Buffer indices_buffer = {{1, 1, 1, 1, 1, 1}, 6};
    renderer_init_buffers(&renderer, &mesh.vertices, &indices_buffer);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    PerspectiveCamera camera = {
        .position = {50.0f, 50.0f, 50.0f},
        .yaw = 45.0f,
        .pitch = 30.0f,
        .speed = 2.5f,
        .sensitivity = 0.1f,
        .z_near = 0.1f,
        .z_far = 500.0f,
        .fov_half_degrees = 45.0f,
        .aspect = (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT),
        .last_cursor_position_x = (float)xpos,
        .last_cursor_position_y = (float)ypos
    };
    
    glfwSetWindowUserPointer(window, &camera);

    Mat4x4f model = MAT4X4F_IDENTITY_INITIALIZER;

    Vec3f rot_axis = {0.0f, 1.0f, 1.0f};
    vec3f_normalize(rot_axis);
    Quat rot = {0};
    quat(rot_axis, degrees_to_rad(0.5f), rot);
    Vec3f scale = {10.0f, 10.0f, 10.0f};
    mat4x4f_scale(model, scale);
    Vec3f trans = {0.1f, 0.0f, 0.0f};

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // for (size_t i = 0; i < mesh.vertices.count; ++i) {
        //     quat_rotate(rot, mesh.vertices.data[i].pos, mesh.vertices.data[i].pos);
        // }
        camera_update(&camera);
        renderer_draw_triangles(&renderer, model, camera.view, camera.projection, camera.position,
                                &mesh.vertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

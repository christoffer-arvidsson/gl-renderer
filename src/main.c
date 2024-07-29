#include "arena.h"
#include "camera.h"
#include "line.h"
#include "matrix.h"
#include "mesh.h"
#include "quat.h"
#include "shader.h"
#include "stdbool.h"
#include "stddef.h"
#include "trig.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "teapot.h"
#include "vertex.h"

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SCREEN_WIDTH 1920U
#define SCREEN_HEIGHT 1080U

#define REGION_SIZE (256 * 1028 * 1028)

Vertex AXIS_LINES[] = {
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={1.0f, 0.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={1.0f, 0.0f, 0.0f, 1.0f}, .color={1.0f, 0.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={0.0f, 1.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 1.0f, 0.0f, 1.0f}, .color={0.0f, 1.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={0.0f, 0.0f, 1.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 1.0f, 1.0f}, .color={0.0f, 0.0f, 1.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
};

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

    camera->pitch = fmaxf(fminf(camera->pitch, 89.0f), -89.0f);

    camera_update(camera);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    (void)mods;
    (void)scancode;
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


int main() {
    GLFWwindow *window = create_window();

    Region allocator = region_alloc_alloc(REGION_SIZE);

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

    MeshRenderer mesh_renderer = {
        .material = material,
        .light = light,
        .mesh = load_teapot_vertices(&allocator, "assets/meshes/teapot_large.txt")
    };
    mesh_renderer_init(&mesh_renderer);

    LinesRenderer axis_renderer = {
        .resolution = {SCREEN_WIDTH, SCREEN_HEIGHT},
        .thickness = 2.0f,
        .aa_radius = {3.0f, 3.0f}
    };
    axis_renderer.vertices = vertex_buffer_alloc(&allocator, 6U);
    for (size_t i = 0U; i < 6U; ++i) {
        vertex_buffer_push(&axis_renderer.vertices, &AXIS_LINES[i]);
    }
    lines_renderer_init(&axis_renderer);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    PerspectiveCamera camera = {.position = {50.0f, 50.0f, 50.0f},
        .yaw = 45.0f,
        .pitch = 30.0f,
        .speed = 2.5f,
        .sensitivity = 0.1f,
        .z_near = 0.1f,
        .z_far = 500.0f,
        .fov_half_degrees = 45.0f,
        .aspect = (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT),
        .viewport = {SCREEN_WIDTH, SCREEN_HEIGHT},
        .last_cursor_position_x = (float)xpos,
        .last_cursor_position_y = (float)ypos};

    glfwSetWindowUserPointer(window, &camera);

    Vec3f rot_axis = {0.0f, 0.0f, 1.0f};
    Vec3f rot_axis2 = {0.0f, 1.0f, 0.5f};
    vec3f_normalize(rot_axis);
    Quat rot_start = {0};
    Quat rot_end = {0};
    quat(rot_axis, degrees_to_rad(0.0f), rot_start);
    quat(rot_axis2, degrees_to_rad(180.0f), rot_end);

    // Vec3f scale = {10.0f, 10.0f, 10.0f};
    // mat4x4f_scale(model, scale);
    // Vec3f trans = {0.1f, 0.0f, 0.0f};

    float t = 0.0f;
    const float dt = 0.005f;
    while (!glfwWindowShouldClose(window)) {
        Mat4x4f model = MAT4X4F_IDENTITY_INITIALIZER;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Quat rot = {0};

        t = fmodf(t + dt, 1.0f);
        quat_slerp(rot_start, rot_end, t, rot);

        // mat4x4f_rotate(model, rot_axis, degrees_to_rad(180.0f) * t);
        mat4x4f_rotate_q(model, rot);

        camera_update(&camera);
        mesh_renderer_draw(&mesh_renderer, &camera, model);
        lines_renderer_draw(&axis_renderer, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

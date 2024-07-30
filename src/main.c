#include "arena.h"
#include "constants.h"
#include "camera.h"
#include "scene.h"
#include "scenes/scene_plotting.h"
#include "scenes/scene_teapot.h"
#include "shader.h"
#include "stdbool.h"
#include "stddef.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define REGION_SIZE (256 * 1028 * 1028)

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
    GLFWwindow* window = create_window();

    Region allocator = region_alloc_alloc(REGION_SIZE);

    Scene scene = plotting_scene_create(window);
    // Scene scene = teapot_scene_create(window);

    scene.init_fn(&allocator, &scene);

    float t = 0.0f;
    const float dt = 0.005f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        t = fmodf(t + dt, 1.0f);

        scene.update_fn(&scene, t);
        scene.render_fn(&scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene.deinit_fn(&scene);

    return 0;
}


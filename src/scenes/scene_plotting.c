
#include "scene_plotting.h"

#include "../constants.h"
#include "../arena.h"
#include "../scene.h"
#include "../shape.h"

void plotting_scene_init(Region* allocator, Scene* scene) {
    scene->line_renderer = (LinesRenderer) {
        .resolution = {SCREEN_WIDTH, SCREEN_HEIGHT},
        .thickness = 3.0f,
        .aa_radius = {2.0f, 0.0f}
    };

    size_t n_samples = 1000U;
    scene->line_renderer.vertices = vertex_buffer_alloc(allocator, n_samples * 2U);
    generate_quadratic_bezier_vertices(&scene->line_renderer.vertices, 5.0f, 3.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_cubic_bezier_vertices(&scene->line_renderer.vertices, 5.0f, 3.0f, 8.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_polyline_vertices(&scene->line_renderer.vertices, -5.0f, 5.0f, n_samples / 4U);
    lines_renderer_init(&scene->line_renderer);

    double xpos, ypos;
    glfwGetCursorPos(scene->window, &xpos, &ypos);

    scene->camera.position[0] = 50.0f;
    scene->camera.position[1] = 50.0f;
    scene->camera.position[2] = 50.0f;
    scene->camera.yaw = 45.0f,
    scene->camera.pitch = 30.0f,
    scene->camera.speed = 2.5f,
    scene->camera.sensitivity = 0.1f,
    scene->camera.z_near = 0.1f,
    scene->camera.z_far = 500.0f,
    scene->camera.fov_half_degrees = 45.0f,
    scene->camera.aspect = (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT),
    scene->camera.viewport[0] = SCREEN_WIDTH,
    scene->camera.viewport[1] = SCREEN_HEIGHT,
    scene->camera.last_cursor_position_x = (float)xpos,
    scene->camera.last_cursor_position_y = (float)ypos;

    glfwSetWindowUserPointer(scene->window, &scene->camera);
}
void plotting_scene_render(Scene* scene) {
    lines_renderer_draw(&scene->line_renderer, &scene->camera);
}
void plotting_scene_update(Scene* scene, float t) {
    camera_update(&scene->camera);
}
void plotting_scene_deinit(Scene* scene) {
    // todo
}

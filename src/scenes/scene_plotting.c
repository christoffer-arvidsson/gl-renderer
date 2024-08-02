
#include "scene_plotting.h"

#include "../constants.h"
#include "../arena.h"
#include "../scene.h"
#include "../shape.h"

typedef struct {
    LinesRenderer line_renderer;
} PlottingState;

void plotting_scene_init(Region* allocator, Scene* scene) {
    scene->state = region_alloc(allocator, sizeof(PlottingState));
    PlottingState* state = (PlottingState*) scene->state;
    state->line_renderer = (LinesRenderer) {
        .resolution = {SCREEN_WIDTH, SCREEN_HEIGHT},
        .thickness = 3.0f,
        .aa_radius = {2.0f, 0.0f}
    };

    size_t n_samples = 1000U;
    state->line_renderer.vertices = vertex_buffer_alloc(allocator, n_samples * 2U);
    generate_quadratic_bezier_vertices(&state->line_renderer.vertices, 5.0f, 3.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_cubic_bezier_vertices(&state->line_renderer.vertices, 5.0f, 3.0f, 8.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_polyline_vertices(&state->line_renderer.vertices, -5.0f, 5.0f, n_samples / 4U);
    lines_renderer_init(&state->line_renderer);

    double xpos, ypos;
    glfwGetCursorPos(scene->window, &xpos, &ypos);

    scene->camera = (PerspectiveCamera) {
        .position = {50.0f, 50.0f, 50.0f},
        .yaw = 45.0f,
        .pitch = 30.0f,
        .speed = 2.5f,
        .sensitivity = 0.1f,
        .zoom_sensitivity = 3.0f,
        .z_near = 0.1f,
        .z_far = 500.0f,
        .fov_half_degrees = 45.0f,
        .aspect = (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT),
        .viewport = {SCREEN_WIDTH, SCREEN_HEIGHT},
        .last_cursor_position_x = (float)xpos,
        .last_cursor_position_y = (float)ypos,
    };

    glfwSetWindowUserPointer(scene->window, &scene->camera);
}
void plotting_scene_render(Scene* scene) {
    PlottingState* state = (PlottingState*) scene->state;
    lines_renderer_draw(&state->line_renderer, &scene->camera);
}
void plotting_scene_update(Scene* scene, float t) {
    camera_update(&scene->camera);
}
void plotting_scene_deinit(Scene* scene) {
    // todo
}

Scene plotting_scene_create(GLFWwindow* window) {
    return (Scene){
        .window = window,
        .init_fn = &plotting_scene_init,
        .render_fn = &plotting_scene_render,
        .update_fn = &plotting_scene_update,
        .deinit_fn = &plotting_scene_deinit,
    };
}

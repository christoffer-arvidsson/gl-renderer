#include "scene_plotting.h"

#include "../arena.h"
#include "../constants.h"
#include "../linalg.h"
#include "../scene.h"

typedef struct {
    LinesRenderer line_renderer;
} PlottingState;

void plotting_scene_init(Region *allocator, Scene *scene) {
    scene->state = region_alloc(allocator, sizeof(PlottingState));
    PlottingState *state = (PlottingState *)scene->state;
    state->line_renderer =
        (LinesRenderer){.resolution = {SCREEN_WIDTH, SCREEN_HEIGHT},
                        .thickness = 3.0f,
                        .aa_radius = {2.0f, 0.0f}};

    size_t n_samples = 1000U;
    state->line_renderer.vertices =
        vertex_buffer_alloc(allocator, n_samples * 2U);
    generate_quadratic_bezier_vertices(&state->line_renderer.vertices, 5.0f,
                                       3.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_cubic_bezier_vertices(&state->line_renderer.vertices, 5.0f, 3.0f,
                                   8.0f, 8.0f, 0.0f, 1.0f, n_samples / 4U);
    generate_polyline_vertices(&state->line_renderer.vertices, -5.0f, 5.0f,
                               n_samples / 4U);
    lines_renderer_init(&state->line_renderer);

    double xpos, ypos;
    glfwGetCursorPos(scene->window, &xpos, &ypos);
    camera_init_default(&scene->camera);
    scene->camera.last_cursor_position_x = xpos;
    scene->camera.last_cursor_position_y = ypos;

    glfwSetWindowUserPointer(scene->window, &scene->camera);
}
void plotting_scene_render(Scene *scene) {
    PlottingState *state = (PlottingState *)scene->state;
    lines_renderer_draw(&state->line_renderer, &scene->camera);
}
void plotting_scene_update(Scene *scene, float t) {
    (void)t;
    camera_update(&scene->camera);
}
void plotting_scene_deinit(Scene *scene) { (void)scene; }

Scene plotting_scene_create(GLFWwindow *window) {
    return (Scene){
        .window = window,
        .init_fn = &plotting_scene_init,
        .render_fn = &plotting_scene_render,
        .update_fn = &plotting_scene_update,
        .deinit_fn = &plotting_scene_deinit,
    };
}

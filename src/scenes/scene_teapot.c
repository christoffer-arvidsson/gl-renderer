#include "scene_teapot.h"
#include <GLFW/glfw3.h>
#include "../arena.h"
#include "../constants.h"
#include "../scene.h"
#include "../mesh.h"

Vertex AXIS_LINES[] = {
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={1.0f, 0.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={1.0f, 0.0f, 0.0f, 1.0f}, .color={1.0f, 0.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={0.0f, 1.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 1.0f, 0.0f, 1.0f}, .color={0.0f, 1.0f, 0.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 0.0f, 1.0f}, .color={0.0f, 0.0f, 1.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
    {.pos={0.0f, 0.0f, 1.0f, 1.0f}, .color={0.0f, 0.0f, 1.0f, 1.0f}, .normal={0.0f, 0.0f, 0.0f, 1.0f}},
};

Mesh load_teapot_vertices(Region* allocator, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        assert(0);
    }

    size_t num_triangles = 0U;
    fscanf(file, "%zul", &num_triangles);

    // Skip the rest of the line after reading num_triangles
    while (fgetc(file) != '\n');

    size_t num_vertices = num_triangles * 3U;
    Mesh mesh = mesh_alloc(allocator, num_vertices);

    for (size_t i = 0U; i < num_triangles; ++i) {
        for (size_t v = 0U; v < 3U; ++v) {

            Vertex vert = {
                .pos = {0.0f, 0.0f, 0.0f, 1.0f},
                .color = {1.0f, 1.0f, 1.0f, 1.0f},
                .normal = {0.0f, 0.0f, 0.0f, 1.0f}
            };  // White for now
            if (fscanf(file, "%f %f %f\n%f %f %f",
                       &vert.pos[0], &vert.pos[1], &vert.pos[2],
                       &vert.normal[0], &vert.normal[1], &vert.normal[2]
                       ) != 6U) {
                fprintf(stderr, "Error reading vertex data at triangle %zu\n", i);
                fclose(file);
                assert(0);
            }
            vertex_buffer_push(&mesh.vertices, &vert);
        }
    }

    fclose(file);
    return mesh;
}


void generate_axis_vertices(VertexBuffer* vb) {
    for (size_t i = 0U; i < 6U; ++i) {
        vertex_buffer_push(vb, &AXIS_LINES[i]);
    }
}

void teapot_scene_init(Region* allocator, Scene* scene) { 
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

    scene->mesh_renderer.material = material;
    scene->mesh_renderer.light = light;
    scene->mesh_renderer.mesh = load_teapot_vertices(allocator, "assets/meshes/teapot_large.txt");
    mesh_renderer_init(&scene->mesh_renderer);
    mat4x4f_set_identity(scene->mesh_renderer.model);

    scene->axis_renderer.resolution[0] = SCREEN_WIDTH;
    scene->axis_renderer.resolution[1] = SCREEN_HEIGHT;
    scene->axis_renderer.thickness = 3.0f;
    scene->axis_renderer.aa_radius[0] = 1.0f;
    scene->axis_renderer.aa_radius[1] = 1.0f;
    scene->axis_renderer.vertices = vertex_buffer_alloc(allocator, 6U);
    generate_axis_vertices(&scene->axis_renderer.vertices);
    lines_renderer_init(&scene->axis_renderer);

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

void teapot_scene_render(Scene* scene) { 
    mesh_renderer_draw(&scene->mesh_renderer, &scene->camera);
    lines_renderer_draw(&scene->axis_renderer, &scene->camera);
}
void teapot_scene_update(Scene* scene, float t) { 
    mat4x4f_set_identity(scene->mesh_renderer.model);
    Vec3f rot_axis = {0.0f, 0.0f, 1.0f};
    Vec3f rot_axis2 = {0.0f, 1.0f, 0.5f};
    vec3f_normalize(rot_axis);

    Quat rot_start = {0};
    Quat rot_end = {0};
    quat(rot_axis, degrees_to_rad(0.0f), rot_start);
    quat(rot_axis2, degrees_to_rad(180.0f), rot_end);

    Quat rot = {0};
    quat_slerp(rot_start, rot_end, t, rot);
    mat4x4f_rotate(scene->mesh_renderer.model, rot_axis, degrees_to_rad(180.0f) * t);
    mat4x4f_rotate_q(scene->mesh_renderer.model, rot);

    camera_update(&scene->camera);
}
void teapot_scene_deinit(Scene* scene) { 
    // free stuff here
}


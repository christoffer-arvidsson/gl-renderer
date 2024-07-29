#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "mesh.h"
#include "vertex.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Mesh load_teapot_vertices(Region* allocator, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        assert(false);
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
                .color = {1.0f, 1.0f, 1.0f}
            };  // White for now
            if (fscanf(file, "%f %f %f\n%f %f %f",
                       &vert.pos[0], &vert.pos[1], &vert.pos[2],
                       &vert.normal[0], &vert.normal[1], &vert.normal[2]
                       ) != 6U) {
                fprintf(stderr, "Error reading vertex data at triangle %zu\n", i);
                fclose(file);
                assert(false);
            }
            vertex_buffer_push(&mesh.vertices, &vert);
        }
    }

    fclose(file);
    return mesh;
}

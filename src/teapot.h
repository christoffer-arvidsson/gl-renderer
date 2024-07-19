#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>

Mesh* load_teapot_vertices(const char *filename, Mesh* mesh) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    size_t num_triangles = 0U;
    fscanf(file, "%zul", &num_triangles);

    // Skip the rest of the line after reading num_triangles
    while (fgetc(file) != '\n');

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
                free(mesh);
                fclose(file);
                return NULL;
            }
            vertex_buffer_push(&mesh->vertices, &vert);
        }
    }

    fclose(file);
    return mesh;
}

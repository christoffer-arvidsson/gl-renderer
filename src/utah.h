#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Vertex v1, v2, v3;
} Triangle;

Triangle* load_teapot_vertices(const char *filename, size_t *num_triangles) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    fscanf(file, "%zul", num_triangles);

    Triangle *triangles = (Triangle*)malloc(sizeof(Triangle) * (*num_triangles));
    if (triangles == NULL) {
        fprintf(stderr, "Could not allocate memory for triangles\n");
        fclose(file);
        return NULL;
    }

    // Skip the rest of the line after reading numTriangles
    while (fgetc(file) != '\n');

    for (int i = 0; i < *num_triangles; i++) {
        if (fscanf(file, "%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f\n%f %f %f",
                   &triangles[i].v1.pos[0], &triangles[i].v1.pos[1], &triangles[i].v1.pos[2],
                   &triangles[i].v1.normal[0], &triangles[i].v1.normal[1], &triangles[i].v1.normal[2],
                   &triangles[i].v2.pos[0], &triangles[i].v2.pos[1], &triangles[i].v2.pos[2],
                   &triangles[i].v2.normal[0], &triangles[i].v2.normal[1], &triangles[i].v2.normal[2],
                   &triangles[i].v3.pos[0], &triangles[i].v3.pos[1], &triangles[i].v3.pos[2],
                   &triangles[i].v3.normal[0], &triangles[i].v3.normal[1], &triangles[i].v3.normal[2]
                   ) != 3U * 6U) {
            fprintf(stderr, "Error reading triangle data at triangle %d\n", i);
            free(triangles);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return triangles;
}

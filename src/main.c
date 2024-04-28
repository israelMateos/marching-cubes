#include "marching_cubes.h"
#include "geometry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    /* Parse command line arguments */
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> <isovalue> <max_grid_size> <output_filename>\n", argv[0]);
        return -1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open input file %s\n", argv[1]);
        return -1;
    }

    float isovalue = atof(argv[2]);
    int max_grid_size = atoi(argv[3]);

    FILE *output_file = fopen(argv[4], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not open output file %s\n", argv[3]);
        return -1;
    }

    /* Allocate memory for scalar field */
    float ***scalar_field = malloc(max_grid_size * sizeof(float **));
    for (int x = 0; x < max_grid_size; x++) {
        scalar_field[x] = malloc(max_grid_size * sizeof(float *));
        for (int y = 0; y < max_grid_size; y++) {
            scalar_field[x][y] = malloc(max_grid_size * sizeof(float));
        }
    }

    /* Read the scalar field values */
    char line[256];
    int n_vertices = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "element vertex") != NULL) {
            sscanf(line, "element vertex %d", &n_vertices);
            break;
        }
    }
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "end_header") != NULL) {
            break;
        }
    }

    for (int i = 0; i < n_vertices; i++) {
        float x, y, z, val;
        fscanf(file, "%f %f %f %f", &x, &y, &z, &val);
        scalar_field[(int)x][(int)y][(int)z] = val;
    }
    fclose(file);

    /* Iterate through the cubes */
    Triangle triangles[4];
    Triangle *total_triangles = NULL;
    int n_total_triangles = 0;
    int n_triangles = 0;

    printf("Running marching cubes...\n");
    for (int x = 0; x < max_grid_size - 1; x++) {
        for (int y = 0; y < max_grid_size - 1; y++) {
            for (int z = 0; z < max_grid_size - 1; z++) {
                if (y == 99)
                    printf("x: %d, y: %d, z: %d\n", x, y, z);
                int cube_index = 0;
                float grid_vals[8];
                Point grid_cell[8] = {
                    {x, y, z},
                    {x + 1, y, z},
                    {x + 1, y + 1, z},
                    {x, y + 1, z},
                    {x, y, z + 1},
                    {x + 1, y, z + 1},
                    {x + 1, y + 1, z + 1},
                    {x, y + 1, z + 1}
                };

                /* Get the scalar values at each corner */
                for (int i = 0; i < 8; i++) {
                    grid_vals[i] = scalar_field[(int)grid_cell[i].x][(int)grid_cell[i].y][(int)grid_cell[i].z];
                    if (grid_vals[i] >= isovalue) {
                        cube_index |= (1 << i);
                    }
                }

                /* Get the triangles */
                n_triangles = marching_cubes(grid_cell, grid_vals, isovalue, triangles, cube_index);
                
                /* Update total triangles */
                total_triangles = realloc(total_triangles,(n_total_triangles + n_triangles)*sizeof(Triangle));
				for (int i = 0; i < n_triangles; i++)
					total_triangles[n_total_triangles + i] = triangles[i];
                n_total_triangles += n_triangles;
            }
        }
    }

    /* Output results to OBJ file */
    fprintf(output_file, "# OBJ file\n");
    fprintf(output_file, "o mesh\n");
    for (int i = 0; i < n_total_triangles; i++) {
        fprintf(output_file, "v %f %f %f\n", total_triangles[i].v1.x, total_triangles[i].v1.y, total_triangles[i].v1.z);
        fprintf(output_file, "v %f %f %f\n", total_triangles[i].v2.x, total_triangles[i].v2.y, total_triangles[i].v2.z);
        fprintf(output_file, "v %f %f %f\n", total_triangles[i].v3.x, total_triangles[i].v3.y, total_triangles[i].v3.z);
    }

    for (int i = 0; i < n_total_triangles; i++) {
        fprintf(output_file, "f %d %d %d\n", 3*i + 1, 3*i + 2, 3*i + 3);
    }
    fclose(output_file);

    /* Free memory */
    for (int x = 0; x < max_grid_size; x++) {
        for (int y = 0; y < max_grid_size; y++) {
            free(scalar_field[x][y]);
        }
        free(scalar_field[x]);
    }
    free(scalar_field);
    free(total_triangles);

    return 0;
}

#include "marching_cubes.h"
#include "geometry.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    /* Parse command line arguments */
    if (argc < 4 || argc > 10) {
        fprintf(stderr, "Usage: %s [-x <x_size> -y <y_size> -z <z_size] <input_filename> <isovalue> <output_filename>\n", argv[0]);
        return -1;
    }

    /* Read options */
    int x_size = 400, y_size = 400, z_size = 400;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-x") == 0) {
            x_size = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-y") == 0) {
            y_size = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-z") == 0) {
            z_size = atoi(argv[i + 1]);
        }
    }

    float isovalue = atof(argv[argc - 2]);
    FILE *output_file = fopen(argv[argc - 1], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error: Could not open output file %s\n", argv[3]);
        return -1;
    }

    /* Allocate memory for scalar field */
    float ***scalar_field = malloc(x_size * sizeof(float **));
    for (int x = 0; x < x_size; x++) {
        scalar_field[x] = malloc(y_size * sizeof(float *));
        for (int y = 0; y < y_size; y++) {
            scalar_field[x][y] = malloc(z_size * sizeof(float));
        }
    }

    /* Read scalar field from file */
    if (strstr(argv[argc - 3], ".ply") != NULL) {
        if (read_scalar_field_from_ply(argv[argc - 3], scalar_field) == -1) {
            return -1;
        }
    } else if (strstr(argv[argc - 3], ".raw") != NULL) {
        if (read_scalar_field_from_raw(argv[argc - 3], x_size, y_size, z_size, scalar_field) == -1) {
            return -1;
        }
    } else {
        fprintf(stderr, "Error: Input file must be a .ply or .raw file\n");
        return -1;
    }
    printf("Scalar field read\n");

    /* Iterate through the cubes */
    Triangle triangles[5];
    Triangle *total_triangles = NULL;
    int n_total_triangles = 0;
    int n_triangles = 0;

    printf("Running marching cubes...\n");
    for (int x = 0; x < x_size - 1; x++) {
        for (int y = 0; y < y_size - 1; y++) {
            for (int z = 0; z < z_size - 1; z++) {
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
    printf("Marching cubes completed\n");

    /* Write triangles to file */
    if (write_triangles(argv[argc - 1], total_triangles, n_total_triangles) == -1) {
        return -1;
    }
    printf("Triangles written to file\n");

    /* Free memory */
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
            free(scalar_field[x][y]);
        }
        free(scalar_field[x]);
    }
    free(scalar_field);
    free(total_triangles);

    return 0;
}

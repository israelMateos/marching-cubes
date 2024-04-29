#include "marching_cubes.h"
#include "geometry.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    /* Parse command line arguments */
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> <isovalue> <max_grid_size> <output_filename>\n", argv[0]);
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

    /* Read scalar field from file */
    if (read_scalar_field(argv[1], scalar_field) == -1) {
        return -1;
    }

    /* Iterate through the cubes */
    Triangle triangles[4];
    Triangle *total_triangles = NULL;
    int n_total_triangles = 0;
    int n_triangles = 0;

    printf("Running marching cubes...\n");
    for (int x = 0; x < max_grid_size - 1; x++) {
        for (int y = 0; y < max_grid_size - 1; y++) {
            for (int z = 0; z < max_grid_size - 1; z++) {
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

    /* Write triangles to file */
    if (write_triangles(argv[4], total_triangles, n_total_triangles) == -1) {
        return -1;
    }

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

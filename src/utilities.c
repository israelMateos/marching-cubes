#include "utilities.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

/*
    * Interpolates between two points based on the isovalue.
    * 
    * edge: The edge to interpolate between.
    * isovalue: The isovalue to interpolate at.
    * start_val: The value at the start of the edge.
    * end_val: The value at the end of the edge.
    * 
    * Returns: The interpolated point.
*/
Point interpolate(Edge edge, float isovalue, float start_val, float end_val) {
    Point start = edge.start;
    Point end = edge.end;
    
    /* Define a threshold for 0 values */
    float threshold = 0.00001;
    if (fabs(isovalue - start_val) < threshold) {
        return start;
    }
    if (fabs(isovalue - end_val) < threshold) {
        return end;
    }
    if (fabs(start_val - end_val) < threshold) {
        return start;
    }
    
    float t = (isovalue - start_val) / (end_val - start_val);
    Point result = {
        .x = start.x + t * (end.x - start.x),
        .y = start.y + t * (end.y - start.y),
        .z = start.z + t * (end.z - start.z)
    };
    
    return result;
}

/*
    * Reads a scalar field from a PLY file.
    *
    * filename: The name of the file to read.
    * scalar_field: The scalar field to read into.
    * 
    * Returns: 0 if successful, -1 if an error occurred.
*/
int read_scalar_field_from_ply(char *filename, float ***scalar_field) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open input file %s\n", filename);
        return -1;
    }

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
    return 0;
}

/*
    * Reads a scalar field from a RAW file.
    *
    * filename: The name of the file to read.
    * x_size: The size of the x dimension.
    * y_size: The size of the y dimension.
    * z_size: The size of the z dimension.
    * scalar_field: The scalar field to read into.
    * 
    * Returns: 0 if successful, -1 if an error occurred.
*/
int read_scalar_field_from_raw(char *filename, int x_size, int y_size, int z_size, float ***scalar_field) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open input file %s\n", filename);
        return -1;
    }

    for (int z = 0; z < z_size; z++) {
        for (int y = 0; y < y_size; y++) {
            for (int x = 0; x < x_size; x++) {
                scalar_field[x][y][z] = fgetc(file);
            }
        }
    }
    
    return 0;
}

int write_triangles(char *filename, Triangle *triangles, int n_triangles) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open output file %s\n", filename);
        return -1;
    }

    fprintf(file, "# OBJ file\n");
    fprintf(file, "o mesh\n");
    for (int i = 0; i < n_triangles; i++) {
        fprintf(file, "v %f %f %f\n", triangles[i].v1.x, triangles[i].v1.y, triangles[i].v1.z);
        fprintf(file, "v %f %f %f\n", triangles[i].v2.x, triangles[i].v2.y, triangles[i].v2.z);
        fprintf(file, "v %f %f %f\n", triangles[i].v3.x, triangles[i].v3.y, triangles[i].v3.z);
    }

    for (int i = 0; i < n_triangles; i++) {
        fprintf(file, "f %d %d %d\n", 3*i + 1, 3*i + 2, 3*i + 3);
    }

    fclose(file);
    return 0;
}
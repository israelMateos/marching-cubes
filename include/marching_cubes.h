#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include "geometry.h"

extern const int edge_table[256];
extern const int triangle_table[256][16];

int marching_cubes(Point grid_cell[8], float grid_vals[8], float isovalue, Triangle *triangles, int cube_index);

#endif

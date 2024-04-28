#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include "geometry.h"

extern int edgeTable[256];

int marching_cubes(Point grid_cell[8], float grid_vals[8], float isovalue, Triangle *triangles, int cube_index);

#endif

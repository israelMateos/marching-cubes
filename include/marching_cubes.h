#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include "geometry.h"

extern int edgeTable[256];

void marching_cubes(float ***scalar_field, int nx, int ny, int nz, float isovalue, Triangle *triangles, int *triangle_count);

#endif

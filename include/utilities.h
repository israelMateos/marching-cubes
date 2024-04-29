#ifndef UTILITIES_H
#define UTILITIES_H

#include "geometry.h"

Point interpolate(Edge edge, float isovalue, float start_val, float end_val);
int read_scalar_field(char *filename, float ***scalar_field);
int write_triangles(char *filename, Triangle *triangles, int n_triangles);

#endif
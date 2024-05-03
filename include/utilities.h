#ifndef UTILITIES_H
#define UTILITIES_H

#include "geometry.h"

Point interpolate(Edge edge, float isovalue, float start_val, float end_val);
int read_scalar_field_from_raw(char *filename, int x_size, int y_size, int z_size, float ***scalar_field);
int read_scalar_field_from_ply(char *filename, float ***scalar_field);
int write_triangles(char *filename, Triangle *triangles, int n_triangles);

#endif
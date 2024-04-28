#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct {
    float x;
    float y;
    float z;
} Point;

typedef struct {
    Point start;
    Point end;
} Edge;

typedef struct {
    Point v1;
    Point v2;
    Point v3;
} Triangle;

#endif
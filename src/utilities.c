#include "utilities.h"
#include <math.h>

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

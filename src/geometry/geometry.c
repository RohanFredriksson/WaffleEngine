#include <math.h>
#include "external.h"
#include "geometry.h"

float Geometry_PerpendicularDistance(vec2 start, vec2 stop, vec2 point) {

    // If the line is vertical, just compute the x difference.
    if (stop[0] == start[0]) {
        return fabs(start[0] - point[0]);
    }

    // If the line is horizontal, just compute the y difference.
    if (stop[1] == start[1]) {
        return fabs(start[1] - point[1]);
    }

    // Get the coefficients of the general line equation.
    float m = (stop[1] - start[1]) / (stop[0] - start[0]);
    float a = m;
    float b = -1.0f;
    float c = start[1] - m * start[0];

    // Perpendicular distance formula.
    return fabs(a * point[0] + b * point[1] + c) / sqrtf(a * a + b * b);

}

bool Geometry_IsAboveLine(vec2 start, vec2 stop, vec2 point) {

    if (stop[0] == start[0]) {
        return -1.0f;
    }

    // y = mx + c
    float m = (stop[1] - start[1]) / (stop[0] - start[0]);
    float c = start[1] - m * start[0];
    float y = m * point[0] + c;

    return point[1] > y;

}

bool Geometry_IsBelowLine(vec2 start, vec2 stop, vec2 point) {
    
    if (stop[0] == start[0]) {
        return -1.0f;
    }

    // y = mx + c
    float m = (stop[1] - start[1]) / (stop[0] - start[0]);
    float c = start[1] - m * start[0];
    float y = m * point[0] + c;

    return point[1] < y;

}

bool Geometry_IsLeftOfLine(vec2 start, vec2 stop, vec2 point) {
    
    if (stop[1] == start[1]) {
        return -1.0f;
    }

    // x = my + c
    float m = (stop[0] - start[0]) / (stop[1] - start[1]);
    float c = start[0] - m * start[1];
    float x = m * point[1] + c;

    return point[0] < x;

}

bool Geometry_IsRightOfLine(vec2 start, vec2 stop, vec2 point) {
    
    if (stop[1] == start[1]) {
        return -1.0f;
    }

    // x = my + c
    float m = (stop[0] - start[0]) / (stop[1] - start[1]);
    float c = start[0] - m * start[1];
    float x = m * point[1] + c;

    return point[0] > x;

}
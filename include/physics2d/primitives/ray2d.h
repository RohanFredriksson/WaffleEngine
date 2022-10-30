#include "external.h"

#ifndef RAY2D_H
#define RAY2D_H

struct Ray2D {
    vec2 origin;
    vec2 direction;
};
typedef struct Ray2D Ray2D;

void Ray2D_Init(Ray2D* ray, vec2 origin, vec2 direction);

#endif
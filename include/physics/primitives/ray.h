#include "external.h"

#ifndef RAY_H
#define RAY_H

struct Ray {
    vec2 origin;
    vec2 direction;
};
typedef struct Ray Ray;

void Ray_Init(Ray* ray, vec2 origin, vec2 direction);

#endif
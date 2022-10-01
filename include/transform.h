#include <stdbool.h>
#include "external.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform {
    vec2 pos;
    vec2 size;
    float rotation;
};
typedef struct Transform Transform;

void Transform_Init(Transform* t, vec2 pos, vec2 size, float rotation);

bool Transform_Equals(Transform t1, Transform t2);

#endif
#include <stdbool.h>
#include "external.h"

#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H

struct RaycastResult {
    vec2 point;
    vec2 normal;
    float t;
    bool hit;
};
typedef struct RaycastResult RaycastResult;

void RaycastResult_Default(RaycastResult* result);

void RaycastResult_Init(RaycastResult* result, vec2 point, vec2 normal, float t, bool hit);

void RaycastResult_Reset(RaycastResult* result);

#endif
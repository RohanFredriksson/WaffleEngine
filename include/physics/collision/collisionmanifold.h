#include <stdbool.h>
#include "external.h"

#ifndef COLLISIONMANIFOLD_H
#define COLLISIONMANIFOLD_H

struct CollisionManifold {
    bool isColliding;
    vec2 normal;
    vec2 contactPoint;
    float depth;
};
typedef struct CollisionManifold CollisionManifold;

void CollisionManifold_Init(CollisionManifold* cm);

void CollisionManifold_Set(CollisionManifold* cm, vec2 normal, vec2 contactPoint, float depth);

#endif
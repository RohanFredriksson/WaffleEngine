#include <stdbool.h>
#include "external.h"

#ifndef COLLISIONMANIFOLD_H
#define COLLISIONMANIFOLD_H

struct CollisionManifold {
    bool isColliding;
    vec2 normal;
    vec2* contactPoints;
    int numContactPoints;
    int sizeContactPoints;
    float depth;
};
typedef struct CollisionManifold CollisionManifold;

void CollisionManifold_Init(CollisionManifold* cm);

void CollisionManifold_Set(CollisionManifold* cm, vec2 normal, vec2* contactPoints, int numContactPoints, float depth);

void CollisionManifold_AddContactPoint(CollisionManifold* cm, vec2 contactPoint);

void CollisionManifold_Free(CollisionManifold* cm);

#endif
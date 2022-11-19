#include <stdlib.h>
#include <string.h>
#include "collisionmanifold.h"

#define INITIAL_CONTACTPOINTS_SIZE 8

void CollisionManifold_Init(CollisionManifold* cm) {
    cm->isColliding = 0;
    glm_vec2_zero(cm->normal);
    glm_vec2_zero(cm->contactPoint);
    cm->depth = 0.0f;
}

void CollisionManifold_Set(CollisionManifold* cm, vec2 normal, vec2 contactPoint, float depth) {
    glm_vec2_copy(normal, cm->normal);
    glm_vec2_copy(contactPoint, cm->contactPoint);
    cm->depth = depth;
    cm->isColliding = 1;
}
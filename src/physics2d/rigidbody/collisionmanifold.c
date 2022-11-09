#include <stdlib.h>
#include <string.h>
#include "collisionmanifold.h"

#define INITIAL_CONTACTPOINTS_SIZE 8

void CollisionManifold_Init(CollisionManifold* cm) {
    cm->isColliding = 0;
    glm_vec2_zero(cm->normal);
    cm->contactPoints = malloc(INITIAL_CONTACTPOINTS_SIZE * sizeof(vec2));
    cm->numContactPoints = 0;
    cm->sizeContactPoints = INITIAL_CONTACTPOINTS_SIZE;
    cm->depth = 0.0f;
}

void CollisionManifold_Set(CollisionManifold* cm, vec2 normal, vec2* contactPoints, int numContactPoints, float depth) {

    glm_vec2_copy(normal, cm->normal);
    for (int i = 0; i < numContactPoints; i++) {CollisionManifold_AddContactPoint(cm, contactPoints[i]);}
    cm->depth = depth;
    cm->isColliding = 1;
    
}

void CollisionManifold_AddContactPoint(CollisionManifold* cm, vec2 contactPoint) {

    if (cm->numContactPoints >= cm->sizeContactPoints) {
        cm->contactPoints = realloc(cm->contactPoints, 2 * cm->sizeContactPoints * sizeof(vec2));
        cm->sizeContactPoints = cm->sizeContactPoints * 2;
    }

    glm_vec2_copy(cm->contactPoints[cm->numContactPoints], contactPoint);
    cm->numContactPoints++;

}

void CollisionManifold_Free(CollisionManifold* cm) {
    free(cm->contactPoints);
}
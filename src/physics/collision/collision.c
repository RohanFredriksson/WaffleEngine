#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wmath.h"
#include "collision.h"
#include "collider.h"

CollisionManifold* Collision_FindCollisionFeatures(Collider* c1, Collider* c2) {

    if (strcmp(c1->type, "Box") == 0 && strcmp(c2->type, "Box") == 0) {
        return Collision_FindCollisionFeaturesBoxAndBox((Box*) c1->data, (Box*) c2->data);
    }

    if (strcmp(c1->type, "Circle") == 0 && strcmp(c2->type, "Circle") == 0) {
        return Collision_FindCollisionFeaturesCircleAndCircle((Circle*) c1->data, (Circle*) c2->data);
    }
    
    return NULL;
}

CollisionManifold* Collision_FindCollisionFeaturesCircleAndCircle(Circle* a, Circle* b) {

    float sumRadii = a->radius + b->radius;
    vec2 distance;
    glm_vec2_sub(b->rigidbody->transform->pos, a->rigidbody->transform->pos, distance);
    if (glm_vec2_norm2(distance) - (sumRadii * sumRadii) > 0) {return NULL;}

    CollisionManifold* result = malloc(sizeof(CollisionManifold));
    CollisionManifold_Init(result);

    // Multiply by 0.5 because we want to separate each circle the same amount
    // Consider updating to factor in the momentum and velocity.
    float depth = fabsf(glm_vec2_norm(distance) - sumRadii) * 0.5f;

    vec2 normal;
    glm_vec2_normalize_to(distance, normal);
    float distanceToPoint = a->radius - depth;

    vec2 contactPoint;
    glm_vec2_scale(normal, distanceToPoint, contactPoint);
    glm_vec2_add(contactPoint, a->rigidbody->transform->pos, contactPoint);

    CollisionManifold_Set(result, normal, &contactPoint, 1, depth);
    return result;

}

CollisionManifold* Collision_FindCollisionFeaturesBoxAndBox(Box* a, Box* b) {

    float depth;
    vec2 normal;
    vec2 contactPoint;

    float xDepths[2];
    float yDepths[2];

    vec2 aMin;
    vec2 bMin;
    vec2 aMax;
    vec2 bMax;

    Box_GetMin(a, aMin);
    Box_GetMin(b, bMin);
    Box_GetMax(a, aMax);
    Box_GetMax(b, bMax);

    bool colliding = aMax[0] > bMin[0] && bMax[0] > aMin[0] && aMax[1] > bMin[1] && bMax[1] > aMin[1];
    if (!colliding) {return NULL;}

    CollisionManifold* result = malloc(sizeof(CollisionManifold));
    CollisionManifold_Init(result);

    xDepths[0] = fabsf(bMax[0] - aMin[0]);
    xDepths[1] = fabsf(bMin[0] - aMax[0]);
    yDepths[0] = fabsf(bMax[1] - aMin[1]);
    yDepths[1] = fabsf(bMin[1] - aMax[1]);

    float minXDepth = WMath_MinFloat(xDepths[0], xDepths[1]);
    float minYDepth = WMath_MinFloat(yDepths[0], yDepths[1]);
    depth = WMath_MinFloat(minXDepth, minYDepth) * 0.5f;

    if (minXDepth < minYDepth) {

        // Determine the normal of collision
        normal[0] = (b->rigidbody->transform->pos[0] > a->rigidbody->transform->pos[0]) ? 1.0f : -1.0f;
        normal[1] = 0.0f;

        // Find the middle of the contact point
        float yValues[4];
        float aY[2];
        float bY[2];
        aY[0] = aMin[1];
        aY[1] = aMax[1];
        bY[0] = bMin[1];
        bY[1] = bMax[1];

        int k = 0;
        int i = 0;
        int j = 0;

        // Combine two sorted arrays
        while (i < 2 && j < 2) {
            
            if (i >= 2) {
                yValues[k] = bY[j];
                j++;
            }

            else if (j >= 2) {
                yValues[k] = aY[i];
                i++;
            }

            else if (aY[i] < bY[j]) {
                yValues[k] = aY[i];
                i++;
            }

            else {
                yValues[k] = bY[j];
                j++;
            }

            k++;
        }

        float yMid = (yValues[2] - yValues[1]) * 0.5f;
        if (xDepths[0] < xDepths[1]) {contactPoint[0] = aMin[0] + normal[0] * depth;} 
        else {contactPoint[0] = aMax[0] + normal[0] * depth;}
        contactPoint[1] = yMid;

    }

    else {

        // Determine the normal of collision
        normal[0] = 0.0f;
        normal[1] = (b->rigidbody->transform->pos[1] > a->rigidbody->transform->pos[1]) ? 1.0f : -1.0f;

        // Find the middle of the contact point
        float xValues[4];
        float aX[2];
        float bX[2];
        aX[0] = aMin[0];
        aX[1] = aMax[0];
        bX[0] = bMin[0];
        bX[1] = bMax[0];

        int k = 0;
        int i = 0;
        int j = 0;

        // Combine two sorted arrays
        while (i < 2 && j < 2) {
            
            if (i >= 2) {
                xValues[k] = bX[j];
                j++;
            }

            else if (j >= 2) {
                xValues[k] = aX[i];
                i++;
            }

            else if (aX[i] < bX[j]) {
                xValues[k] = aX[i];
                i++;
            }

            else {
                xValues[k] = bX[j];
                j++;
            }

            k++;
        }

        float xMid = (xValues[2] - xValues[1]) * 0.5f;
        if (yDepths[0] < yDepths[1]) {contactPoint[1] = aMin[1] + normal[1] * depth;} 
        else {contactPoint[1] = aMax[1] + normal[1] * depth;}
        contactPoint[0] = xMid;

    }

    CollisionManifold_Set(result, normal, &contactPoint, 1, depth);
    return result;
}
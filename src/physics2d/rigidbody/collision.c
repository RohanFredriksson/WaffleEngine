#include <stdlib.h>
#include <math.h>
#include "collision.h"

CollisionManifold* Collision_FindCollisionFeaturesCircleAndCircle(Circle a, Circle b) {
    
    CollisionManifold* result = malloc(sizeof(CollisionManifold));
    CollisionManifold_Init(result);

    float sumRadii = a.radius + b.radius;
    vec2 distance;
    glm_vec2_sub(b.rigidbody->transform->pos, a.rigidbody->transform->pos, distance);
    if (glm_vec2_norm2(distance) - (sumRadii * sumRadii) > 0) {return result;}

    // Multiply by 0.5 because we want to separate each circle the same amount
    // Consider updating to factor in the momentum and velocity.
    float depth = fabsf(glm_vec2_norm(distance) - sumRadii) * 0.5f;

    vec2 normal;
    glm_vec2_normalize_to(distance, normal);
    float distanceToPoint = a.radius - depth;

    vec2 contactPoint;
    glm_vec2_scale(normal, distanceToPoint, contactPoint);
    glm_vec2_add(contactPoint, a.rigidbody->transform->pos, contactPoint);

    CollisionManifold_Set(result, normal, &contactPoint, 1, depth);
    return result;

}
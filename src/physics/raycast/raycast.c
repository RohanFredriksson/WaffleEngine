#include <string.h>
#include "external.h"
#include "raycast.h"
#include "wmath.h"

bool Raycast_Rigidbody(Rigidbody* rigidbody, Ray ray, RaycastResult* result) {

    Collider* collider = rigidbody->collider;
    if (collider == NULL) {return 0;}

    if (strcmp(collider->type, "Box") == 0) {
        return Raycast_Box(*((Box*) collider->data), ray, result);
    }

    if (strcmp(collider->type, "Circle") == 0) {
        return Raycast_Circle(*((Circle*) collider->data), ray, result);
    }

    return 0;
}

bool Raycast_Box(Box box, Ray ray, RaycastResult* result) {

    RaycastResult_Reset(result);

    vec2 unitVector;
    glm_vec2_copy(ray.direction, unitVector);
    glm_vec2_normalize(unitVector);
    unitVector[0] = (unitVector != 0) ? 1.0f / unitVector[0] : 0.0f;
    unitVector[1] = (unitVector != 0) ? 1.0f / unitVector[1] : 0.0f;

    vec2 min;
    Box_GetMin(&box, min);
    glm_vec2_sub(min, ray.origin, min);
    glm_vec2_mul(min, unitVector, min);

    vec2 max;
    Box_GetMax(&box, max);
    glm_vec2_sub(max, ray.origin, max);
    glm_vec2_mul(max, unitVector, max);

    float tmin = WMath_MaxFloat(WMath_MinFloat(min[0], max[0]), WMath_MinFloat(min[1], max[1]));
    float tmax = WMath_MinFloat(WMath_MaxFloat(min[0], max[0]), WMath_MaxFloat(min[1], max[1]));
    if (tmax < 0 || tmin > tmax) {
        return 0;
    }

    float t = (tmin < 0.0f) ? tmax : tmin;
    bool hit = t > 0.0f;// && t * t < ray.maximum;
    if (!hit) {
        return 0;
    }

    if (result != NULL) {
        vec2 point;
        glm_vec2_scale(ray.direction, t, point);
        glm_vec2_add(ray.origin, point, point);
        vec2 normal;
        glm_vec2_sub(ray.origin, point, normal);
        glm_vec2_normalize(normal);
        RaycastResult_Init(result, point, normal, t, 1);
    }

    return 1;
}

bool Raycast_Circle(Circle circle, Ray ray, RaycastResult* result) {
    
    RaycastResult_Reset(result);
    
    vec2 originToCircle;
    glm_vec2_sub(circle.rigidbody->component->entity->position, ray.origin, originToCircle);
    float radiusSquared = circle.radius * circle.radius;
    float originToCircleLengthSquared = glm_vec2_norm2(originToCircle);

    // Project the vector from the ray origin onto the direction of the ray
    float a = glm_vec2_dot(originToCircle, ray.direction);
    float b2 = originToCircleLengthSquared - (a * a);
    if (radiusSquared - b2 < 0.0f) {
        return 0;
    }

    float f = sqrtf(radiusSquared - b2);
    float t = 0;
    if (originToCircleLengthSquared < radiusSquared) {t = a + f;} // Ray starts inside the circle
    else {t = a - f;}

    if (result != NULL) {
        vec2 point;
        glm_vec2_scale(ray.direction, t, point);
        glm_vec2_add(ray.origin, point, point);
        vec2 normal;
        glm_vec2_sub(point, circle.rigidbody->component->entity->position, normal);
        glm_vec2_normalize(normal);
        RaycastResult_Init(result, point, normal, t, 1);
    }
    
    return 1;
}
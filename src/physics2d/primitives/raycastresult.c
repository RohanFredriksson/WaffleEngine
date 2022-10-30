#include "raycastresult.h"

void RaycastResult_Default(RaycastResult* result) {
    glm_vec2_zero(result->point);
    glm_vec2_zero(result->normal);
    result->t = 0;
    result->hit = 0;
}

void RaycastResult_Init(RaycastResult* result, vec2 point, vec2 normal, float t, bool hit) {
    glm_vec2_copy(point, result->point);
    glm_vec2_copy(normal, result->normal);
    result->t = t;
    result->hit = hit;
}

void RaycastResult_Reset(RaycastResult* result) {
    if (result != NULL) {
        glm_vec2_zero(result->point);
        glm_vec2_zero(result->normal);
        result->t = 0;
        result->hit = 0;
    }
}
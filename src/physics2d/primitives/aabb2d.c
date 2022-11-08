#include "aabb2d.h"

void AABB2D_Init(AABB2D* box, vec2 size) {
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(size, 0.5f, box->halfSize);
    box->rigidbody = NULL;
}

void AABB2D_InitRange(AABB2D* box, vec2 min, vec2 max) {
    vec2 size;
    glm_vec2_sub(max, min, size);
    AABB2D_Init(box, size);
}

void AABB2D_GetMin(AABB2D* box, vec2 dest) {
    glm_vec2_sub(box->rigidbody->transform->pos, box->halfSize, dest);
}

void AABB2D_GetMax(AABB2D* box, vec2 dest) {
    glm_vec2_add(box->rigidbody->transform->pos, box->halfSize, dest);
}

void AABB2D_SetSize(AABB2D* box, vec2 size) {
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(box->size, 0.5f, box->halfSize);
}

void AABB2D_SetRigidbody(AABB2D* box, Rigidbody2D* rb) {
    box->rigidbody = rb;
}
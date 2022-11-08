#include <stdlib.h>
#include <string.h>
#include "wmath.h"
#include "box2d.h"

void Box2D_Init(Box2D* box, vec2 size) {
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(size, 0.5f, box->halfSize);
    box->rigidbody = NULL;
}

void Box2D_InitRange(Box2D* box, vec2 min, vec2 max) {
    vec2 size;
    glm_vec2_sub(max, min, size);
    Box2D_Init(box, size);
}

void Box2D_GetLocalMin(Box2D* box, vec2 dest) {
    glm_vec2_sub(box->rigidbody->transform->pos, box->halfSize, dest);
}

void Box2D_GetLocalMax(Box2D* box, vec2 dest) {
    glm_vec2_add(box->rigidbody->transform->pos, box->halfSize, dest);
}

void Box2D_GetVertices(Box2D* box, vec2* buffer) {

    vec2 min;
    vec2 max;
    Box2D_GetLocalMin(box, min);
    Box2D_GetLocalMax(box, max);

    vec2 vertices[4];
    vertices[0][0] = min[0];
    vertices[0][1] = min[1];
    vertices[1][0] = min[0];
    vertices[1][1] = max[1];
    vertices[2][0] = max[0];
    vertices[2][1] = min[1];
    vertices[3][0] = max[0];
    vertices[3][1] = max[1];

    if (box->rigidbody->transform->rotation != 0.0f) {
        for (int i = 0; i < 4; i++) {
            WMath_Rotate(vertices[i], box->rigidbody->transform->rotation, box->rigidbody->transform->pos);
        }
    }

    memcpy(vertices, buffer, sizeof(vec2) * 4);
}

void Box2D_SetSize(Box2D* box, vec2 size) {
    glm_vec2_copy(size, box->size);
    glm_vec2_scale(box->size, 0.5f, box->halfSize);
}

void Box2D_SetRigidbody(Box2D* box, Rigidbody2D* rb) {
    box->rigidbody = rb;
}
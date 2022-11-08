#include "external.h"
#include "rigidbody2d.h"

#ifndef AABB2D_H
#define AABB2D_H

struct AABB2D {
    vec2 size;
    vec2 halfSize;
    Rigidbody2D* rigidbody;
};
typedef struct AABB2D AABB2D;

void AABB2D_Init(AABB2D* box, vec2 size);
void AABB2D_InitRange(AABB2D* box, vec2 min, vec2 max);

void AABB2D_GetMin(AABB2D* box, vec2 dest);
void AABB2D_GetMax(AABB2D* box, vec2 dest);

void AABB2D_SetSize(AABB2D* box, vec2 size);
void AABB2D_SetRigidbody(AABB2D* box, Rigidbody2D* rb);

#endif
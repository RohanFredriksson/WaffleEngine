#include "external.h"
#include "rigidbody2d.h"

#ifndef BOX2D_H
#define BOX2D_H

struct Box2D {
    vec2 size;
    vec2 halfSize;
    Rigidbody2D* rigidbody;
};
typedef struct Box2D Box2D;

void Box2D_Init(Box2D* box, vec2 size);
void Box2D_InitRange(Box2D* box, vec2 min, vec2 max);

void Box2D_GetLocalMin(Box2D* box, vec2 dest);
void Box2D_GetLocalMax(Box2D* box, vec2 dest);
void Box2D_GetVertices(Box2D* box, vec2* buffer);

void Box2D_SetSize(Box2D* box, vec2 size);
void Box2D_SetRigidbody(Box2D* box, Rigidbody2D* rb);

#endif
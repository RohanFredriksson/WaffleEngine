#include "external.h"
#include "rigidbody2d.h"

#ifndef BOX2D_H
#define BOX2D_H

struct Box2D {
    Collider2D* collider;
    Rigidbody2D* rigidbody;
    vec2 size;
    vec2 halfSize;
};
typedef struct Box2D Box2D;

Component* Box2D_Init(vec2 size, Component* rigidbody);
void Box2D_Update(Collider2D* c, float dt);
void Box2D_Free(Collider2D* c);

void Box2D_GetLocalMin(Box2D* box, vec2 dest);
void Box2D_GetLocalMax(Box2D* box, vec2 dest);
void Box2D_GetVertices(Box2D* box, vec2* buffer);

void Box2D_SetSize(Box2D* box, vec2 size);
void Box2D_SetRigidbody(Box2D* box, Rigidbody2D* rb);

#endif
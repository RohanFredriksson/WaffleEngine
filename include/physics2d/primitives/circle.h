#include "gameobject.h"
#include "collider2d.h"
#include "rigidbody2d.h"

#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle {
    Collider2D* collider;
    Rigidbody2D* rigidbody;
    float radius;
};
typedef struct Circle Circle;

Component* Circle_Init(float radius, Component* rigidbody);
void Circle_Update(Collider2D* c, float dt);
void Circle_Free(Collider2D* c);

void Circle_SetRadius(Circle* c, float radius);
void Circle_SetRigidbody(Circle* c, Rigidbody2D* rb);

#endif
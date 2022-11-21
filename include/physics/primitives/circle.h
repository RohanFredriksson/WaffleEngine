#include "entity.h"
#include "collider.h"
#include "rigidbody.h"

#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle {
    Collider* collider;
    Rigidbody* rigidbody;
    float radius;
};
typedef struct Circle Circle;

Component* Circle_Init(float radius, Component* rigidbody);

void Circle_SetRadius(Circle* c, float radius);
void Circle_SetRigidbody(Circle* c, Rigidbody* rb);

#endif
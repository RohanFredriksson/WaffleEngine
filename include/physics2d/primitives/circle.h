#include "rigidbody2d.h"

#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle {
    float radius;
    Rigidbody2D* rigidbody;
};
typedef struct Circle Circle;

void Circle_Init(Circle* c, float radius);
void Circle_SetRadius(Circle* c, float radius);
void Circle_SetRigidbody(Circle* c, Rigidbody2D* rb);

#endif
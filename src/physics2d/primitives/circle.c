#include "circle.h"

void Circle_Init(Circle* c, float radius) {
    c->radius = radius;
    c->rigidbody = NULL;
}

void Circle_SetRadius(Circle* c, float radius) {
    c->radius = radius;
}

void Circle_SetRigidbody(Circle* c, Rigidbody2D* rb) {
    c->rigidbody = rb;
}
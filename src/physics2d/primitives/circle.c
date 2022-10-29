#include "circle.h"

void Circle_Init(Circle* c, float radius) {
    c->radius = radius;
    c->rigidbody = NULL;
}
#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle {
    float radius;
};
typedef struct Circle Circle;

void Circle_Init(Circle* c, float radius);

#endif
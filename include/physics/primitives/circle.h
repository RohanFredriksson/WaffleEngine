#include "entity.h"
#include "rigidbody.h"
#include "rigidbody.h"
#include "wio.h"

#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle {
    Collider* collider;
    float radius;
};
typedef struct Circle Circle;

Component* Circle_Init(float radius);
bool Circle_Load(Collider* co, cJSON* json);
Component* Circle_GetRigidbody(Circle* c);
void Circle_SetRadius(Circle* c, float radius);

#endif
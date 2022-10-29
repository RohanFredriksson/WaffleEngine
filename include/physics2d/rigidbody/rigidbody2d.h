#include "external.h"
#include "gameobject.h"

#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

struct Rigidbody2D {
    Component* component;
    vec2 pos;
    float rotation;
};
typedef struct Rigidbody2D Rigidbody2D;

Component* Rigidbody2D_Init(vec2 pos, float rotation);

void Rigidbody2D_Update(Component* c, float dt);

void Rigidbody2D_Free(Component* c);

#endif
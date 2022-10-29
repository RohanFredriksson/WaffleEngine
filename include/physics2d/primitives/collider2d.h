#include "external.h"
#include "gameobject.h"

#ifndef COLLIDER2D_H
#define COLLIDER2D_H

struct Collider2D {
    Component* component;
    vec2 offset;
};
typedef struct Collider2D Collider2D;

Component* Collider2D_Init();

void Collider2D_Update(Component* c, float dt);

void Collider2D_Free(Component* c);

//float Collider2D_GetInertiaTensor(float mass);

#endif
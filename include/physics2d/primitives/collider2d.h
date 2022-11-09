#include "external.h"
#include "gameobject.h"

#ifndef COLLIDER2D_H
#define COLLIDER2D_H

struct Collider2D {
    Component* component;
    vec2 offset;
    void (*update)(struct Collider2D* c, float dt);
    void (*free)(struct Collider2D* c);
    char* type;
    void* data;
};
typedef struct Collider2D Collider2D;

Component* Collider2D_Init(char* type, 
                           void (*update)(Collider2D* c, float dt), 
                           void (*free)(Collider2D* c));

void Collider2D_Update(Component* c, float dt);

void Collider2D_Free(Component* c);

//float Collider2D_GetInertiaTensor(float mass);

#endif
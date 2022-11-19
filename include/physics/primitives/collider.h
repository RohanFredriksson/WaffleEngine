#include "external.h"
#include "gameobject.h"

#ifndef COLLIDER_H
#define COLLIDER_H

struct Collider {
    Component* component;
    vec2 offset;
    void (*update)(struct Collider* c, float dt);
    void (*free)(struct Collider* c);
    char* type;
    void* data;
};
typedef struct Collider Collider;

Component* Collider_Init(char* type, 
                         void (*update)(Collider* c, float dt), 
                         void (*free)(Collider* c));

void Collider_Update(Component* c, float dt);

void Collider_Free(Component* c);

//float Collider_GetInertiaTensor(float mass);

#endif
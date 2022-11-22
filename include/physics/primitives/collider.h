#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "entity.h"

#ifndef COLLIDER_H
#define COLLIDER_H

struct Collider {
    Component* component;
    void (*update)(struct Collider* c, float dt);
    void (*free)(struct Collider* c);
    void (*collision)(struct Collider* c, Entity* with, vec2 contact, vec2 normal);
    cJSON* (*serialise)(struct Collider* c);
    char* type;
    void* data;
};
typedef struct Collider Collider;

Component* Collider_Init(char* type, 
                         void (*update)(Collider* c, float dt), 
                         void (*collision)(Collider* c, Entity* with, vec2 contact, vec2 normal),
                         cJSON* (*serialise)(struct Collider* c),
                         void (*free)(Collider* c));

void Collider_Update(Component* c, float dt);

void Collider_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal);

void Collider_Free(Component* c);

cJSON* Collider_Serialise(Component* c);

//float Collider_GetInertiaTensor(float mass);

#endif
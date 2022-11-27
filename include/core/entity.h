#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "external.h"
#include "list.h"
#include "wmath.h"
#include "wio.h"

#ifndef ENTITY_H
#define ENTITY_H

struct Entity {

    int id;
    bool dead;
    List components;
    vec2 position;
    vec2 size;
    float rotation;
    
};
typedef struct Entity Entity;

struct Component {

    int id;
    bool dead;
    char* type;
    Entity* entity;

    void (*update)(struct Component* c, float dt);
    void (*collision)(struct Component* c, struct Entity* with, vec2 contact, vec2 normal);
    cJSON* (*serialise) (struct Component* c);
    void (*free)(struct Component* c);

    vec2 positionOffset;
    vec2 sizeScale;
    float rotationOffset;

    void* data;

};
typedef struct Component Component;

Entity* Entity_Init(vec2 position, vec2 size, float rotation);

void Entity_Update(Entity* e, float dt);

void Entity_OnCollision(Entity* e, Entity* with, vec2 contact, vec2 normal);

cJSON* Entity_Serialise(Entity* e);

void Entity_Free(Entity* e);

void Entity_Kill(Entity* e);

Entity* Entity_Parse(cJSON* json);

Component* Entity_GetComponent(Entity* e, char* type);

Component* Entity_GetComponentByID(Entity* e, int id);

void Entity_AddComponent(Entity* e, Component* c);

Component* Component_Init(char* type);

void Component_Update(Component* c, float dt);

void Component_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal);

cJSON* Component_Serialise(Component* c);

void Component_Free(Component* c);

void Component_Kill(Component* c);

void Component_GetPosition(Component* c, vec2 dest);

void Component_GetSize(Component* c, vec2 dest);

float Component_GetRotation(Component* c);

Component* Component_Load(cJSON* json);

#endif
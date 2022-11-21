#include "external.h"
#include "list.h"

#ifndef ENTITY_H
#define ENTITY_H

struct Entity {

    int id;
    List components;
    vec2 position;
    vec2 size;
    float rotation;
    
};
typedef struct Entity Entity;

struct Component {

    Entity* entity;
    char* type;
    void (*update)(struct Component* c, float dt);
    void (*collision)(struct Component* c, struct Entity* with, vec2 contact, vec2 normal);
    void (*free)(struct Component* c);

    vec2 positionOffset;
    vec2 positionScale;
    vec2 sizeOffset;
    vec2 sizeScale;
    float rotationOffset;
    float rotationScale;
    
    void* data;

};
typedef struct Component Component;

Entity* Entity_Init(vec2 position, vec2 size, float rotation);

void Entity_Update(Entity* e, float dt);

void Entity_OnCollision(Entity* e, Entity* with, vec2 contact, vec2 normal);

void Entity_Free(Entity* e);

void Entity_AddComponent(Entity* e, Component* c);

void Entity_RemoveComponent(Entity* e, Component* c);

Component* Entity_GetComponent(Entity* e, const char* type);

Component* Component_Init(char* type, 
                          void (*update)(Component* c, float dt),
                          void (*collision)(struct Component* c, Entity* with, vec2 contact, vec2 normal), 
                          void (*free)(Component* c));

void Component_Update(Component* c, float dt);

void Component_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal);

void Component_Free(Component* c);

void Component_GetPosition(Component* c, vec2 dest);

void Component_GetSize(Component* c, vec2 dest);

float Component_GetRotation(Component* c);

#endif
#include "transform.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct GameObject {
    int id;
    int numComponents;
    int sizeComponents;
    struct Component* components;
    int transform; // ID of transform in the transform pool.
};
typedef struct GameObject GameObject;

void GameObject_Init(GameObject* g, Transform* t);

void GameObject_Update(GameObject* g, float dt);

void GameObject_Free(GameObject* g);

void GameObject_AddComponent(GameObject* g, struct Component* c);

struct Component {
    int go; // Gameobject id.
    void (*update)(struct Component* c, float dt);
    void (*free)(struct Component* c);
    char* type;
    void* data;
};
typedef struct Component Component;

void Component_Init(Component* c, 
                    char* type, 
                    void (*update)(Component* c, float dt), 
                    void (*free)(Component* c));

void Component_Free(Component* c);

#endif
#include "transform.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct GameObject {
    int id;
    int num_components;
    int size_components;
    struct Component* components;
    Transform* transform;
};
typedef struct GameObject GameObject;

void GameObject_Init(GameObject* g, Transform* t);

void GameObject_Update(GameObject* g, float dt);

void GameObject_Free(GameObject* g);

void GameObject_AddComponent(GameObject* g, struct Component* c);

struct Component {
    struct GameObject* go;
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
#include "transform.h"
#include "list.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

struct GameObject {
    int id;
    List components;
    Transform* transform;
};
typedef struct GameObject GameObject;

struct Component {
    GameObject* go;
    void (*update)(struct Component* c, float dt);
    void (*free)(struct Component* c);
    char* type;
    void* data;
};
typedef struct Component Component;

GameObject* GameObject_Init(Transform* t);

void GameObject_Update(GameObject* g, float dt);

void GameObject_Free(GameObject* g);

void GameObject_AddComponent(GameObject* g, Component* c);

void GameObject_RemoveComponent(GameObject* g, Component* c);

Component* GameObject_GetComponent(GameObject* g, const char* type);

Component* Component_Init(char* type, 
                                 void (*update)(Component* c, float dt), 
                                 void (*free)(Component* c));

void Component_Update(Component* c, float dt);

void Component_Free(Component* c);

#endif
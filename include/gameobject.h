#include "component.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

int next_id;

struct GameObject {
    int id;
    int num_components;
    Component** components;
    Transform* transform;
};
typedef struct GameObject GameObject;

void GameObject_Init(GameObject* g);

void GameObject_Update(GameObject* g, float dt);

void GameObject_Free(GameObject* g);

void GameObject_AddComponent(GameObject* g, Component* c);

#endif
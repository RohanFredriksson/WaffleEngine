#include "gameobject.h"

#ifndef COMPONENT_H
#define COMPONENT_H

struct Component {
    GameObject* go;
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
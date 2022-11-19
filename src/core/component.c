#include <stdlib.h>
#include <string.h>
#include "gameobject.h"

Component* Component_Init(char* type, 
                          void (*update)(Component* c, float dt), 
                          void (*free)(Component* c)) {

    Component* c = (Component*) malloc(sizeof(Component));

    // Store the type
    c->type = malloc(strlen(type)+1);
    memcpy(c->type, type, strlen(type)+1);

    c->update = update;
    c->free = free;

    return c;
}

void Component_Update(Component* c, float dt) {
    if (c->update != NULL) {c->update(c, dt);}
}

void Component_Free(Component* c) {
    c->free(c);
    free(c->data);
    free(c->type);
}
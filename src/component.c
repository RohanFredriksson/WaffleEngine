#include <stdlib.h>
#include <string.h>
#include "gameobject.h"

void Component_Init(Component* c, 
                    char* type, 
                    void (*update)(Component* c, float dt), 
                    void (*free)(Component* c)) {

    // Store the type
    c->type = malloc(strlen(type)+1);
    memcpy(c->type, type, strlen(type)+1);

    c->update = update;
    c->free = free;

}

void Component_Free(Component* c) {
    c->free(c);
    free(c->type);
}
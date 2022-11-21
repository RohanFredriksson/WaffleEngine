#include <stdlib.h>
#include <string.h>
#include "entity.h"

Component* Component_Init(char* type, 
                          void (*update)(Component* c, float dt), 
                          void (*collision)(struct Component* c, struct Entity* with, vec2 contact, vec2 normal),
                          void (*free)(Component* c)) {

    Component* c = (Component*) malloc(sizeof(Component));

    // Store the type
    c->type = malloc(strlen(type)+1);
    memcpy(c->type, type, strlen(type)+1);

    c->update = update;
    c->collision = collision;
    c->free = free;

    return c;
}

void Component_Update(Component* c, float dt) {
    if (c->update != NULL) {c->update(c, dt);}
}

void Component_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal) {
    if (c->collision != NULL) {c->collision(c, with, contact, normal);}
}

void Component_Free(Component* c) {
    if (c->free != NULL) {c->free(c);}
    if (c->data != NULL) {free(c->data);}
    free(c->type);
}
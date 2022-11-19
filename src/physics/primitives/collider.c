#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "collider.h"

Component* Collider_Init(char* type, 
                         void (*update)(Collider* c, float dt), 
                         void (*free)(Collider* c)) {

    Component* c = Component_Init("Collider", &Collider_Update, &Collider_Free);
    Collider* co = malloc(sizeof(Collider));

    co->type = malloc(strlen(type)+1);
    memcpy(co->type, type, strlen(type)+1);
    co->update = update;
    co->free = free;

    co->component = c;
    c->data = co;
    return c;

}

void Collider_Update(Component* c, float dt) {
    Collider* co = (Collider*) c->data;
    if (co->update != NULL) {co->update(co, dt);}
}

void Collider_Free(Component* c) {
    Collider* co = (Collider*) c->data;
    if (co->free != NULL) {co->free(co);}
    if (co->data != NULL) {free(co->data);}
    free(co->type);
}
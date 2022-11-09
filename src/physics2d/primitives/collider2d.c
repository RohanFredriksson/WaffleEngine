#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "collider2d.h"

Component* Collider2D_Init(char* type, 
                           void (*update)(Collider2D* c, float dt), 
                           void (*free)(Collider2D* c)) {

    Component* c = Component_Init("Collider2D", &Collider2D_Update, &Collider2D_Free);
    Collider2D* co = malloc(sizeof(Collider2D));

    co->type = malloc(strlen(type)+1);
    memcpy(co->type, type, strlen(type)+1);
    co->update = update;
    co->free = free;

    co->component = c;
    c->data = co;
    return c;

}

void Collider2D_Update(Component* c, float dt) {

}

void Collider2D_Free(Component* c) {
    Collider2D* co = (Collider2D*) c->data;
    co->free(co);
    free(co->data);
    free(co->type);
}
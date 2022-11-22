#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "collider.h"

Component* Collider_Init(char* type, 
                         void (*update)(Collider* c, float dt), 
                         void (*collision)(Collider* c, Entity* with, vec2 contact, vec2 normal),
                         void (*free)(Collider* c)) {

    Component* c = Component_Init("Collider", &Collider_Update, &Collider_OnCollision, &Collider_Free);
    Collider* co = malloc(sizeof(Collider));

    co->type = type;
    co->update = update;
    co->collision = collision;
    co->free = free;

    co->component = c;
    c->data = co;
    return c;

}

void Collider_Update(Component* c, float dt) {
    Collider* co = (Collider*) c->data;
    if (co->update != NULL) {co->update(co, dt);}
}

void Collider_OnCollision(Component* c, Entity* with, vec2 contact, vec2 normal) {
    Collider* co = (Collider*) c->data;
    if (co->collision != NULL) {co->collision(co, with, contact, normal);}
}

void Collider_Free(Component* c) {
    Collider* co = (Collider*) c->data;
    if (co->free != NULL) {co->free(co);}
    if (co->data != NULL) {free(co->data);}
    free(co->type);
}
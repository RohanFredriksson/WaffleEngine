#include "rigidbody.h"

Component* Collider_Init(Rigidbody* rigidbody,
                         char* type, 
                         void (*update)(Collider* c, float dt), 
                         void (*collision)(Collider* c, Entity* with, vec2 contact, vec2 normal),
                         cJSON* (*serialise)(struct Collider* c),
                         void (*free)(Collider* c)) {

    Component* c = Component_Init("Collider", &Collider_Update, &Collider_OnCollision, &Collider_Serialise, &Collider_Free);
    Collider* co = malloc(sizeof(Collider));

    co->rigidbody = rigidbody->component->id;
    co->type = type;
    co->update = update;
    co->collision = collision;
    co->serialise = serialise;
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
}

Component* Collider_GetRigidbody(Collider* co) {
    return Entity_GetComponentByID(co->component->entity, co->rigidbody);
}

void Collider_SetRigidbody(Collider* co, Rigidbody* rb) {
    co->rigidbody = rb->component->id;
}

cJSON* Collider_Serialise(Component* c) {

    Collider* co = (Collider*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "type", co->type);
    cJSON* child;
    if (co->serialise != NULL) {child = co->serialise(co);}
    else {child = cJSON_CreateNull();} 
    cJSON_AddItemToObject(json, "child", child);
    WIO_AddInt(json, "rigidbody", co->rigidbody);
    return json;

}
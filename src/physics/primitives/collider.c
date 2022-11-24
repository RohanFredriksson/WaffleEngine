#include "rigidbody.h"

Component* Collider_Init(char* type, Rigidbody* rigidbody) {

    Component* c = Component_Init("Collider");
    
    Collider* co = malloc(sizeof(Collider));
    co->update = NULL;
    co->collision = NULL;
    co->serialise = NULL;
    co->free = NULL;

    co->component = c;
    co->type = type;
    co->rigidbody = rigidbody->component->id;
    
    c->update = &Collider_Update;
    c->collision = &Collider_OnCollision;
    c->serialise = &Collider_Serialise;
    c->free = &Collider_Free;
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
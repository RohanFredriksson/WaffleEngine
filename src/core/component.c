#include "entity.h"
#include "components.h"

static int next = 0;

Component* Component_Init(char* type, 
                          void (*update)(Component* c, float dt), 
                          void (*collision)(struct Component* c, struct Entity* with, vec2 contact, vec2 normal),
                          cJSON* (*serialise) (struct Component* c),
                          void (*free)(Component* c)) {

    Component* c = (Component*) malloc(sizeof(Component));

    c->id = next;
    c->type = type;
    glm_vec2_zero(c->positionOffset);
    glm_vec2_one(c->sizeScale);
    c->rotationOffset = 0;

    c->update = update;
    c->collision = collision;
    c->serialise = serialise;
    c->free = free;

    next++;
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
}

void Component_GetPosition(Component* c, vec2 dest) {
    glm_vec2_add(c->entity->position, c->positionOffset, dest);
}

void Component_GetSize(Component* c, vec2 dest) {
    glm_vec2_mul(c->entity->size, c->sizeScale, dest);
}

float Component_GetRotation(Component* c) {
    return c->entity->rotation + c->rotationOffset;
}

cJSON* Component_Serialise(Component* c) {

    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "id", c->id);
    WIO_AddString(json, "type", c->type);
    cJSON* child;
    if (c->serialise != NULL) {child = c->serialise(c);}
    else {child = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "child", child);
    WIO_AddVec2(json, "positionOffset", c->positionOffset);
    WIO_AddVec2(json, "sizeScale", c->sizeScale);
    WIO_AddFloat(json, "rotationOffset", c->rotationOffset);
    return json;

}
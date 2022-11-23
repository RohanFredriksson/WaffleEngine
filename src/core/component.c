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
    free(c->type);
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

    cJSON* id = cJSON_CreateNumber(c->id);
    cJSON_AddItemToObject(json, "id", id);

    cJSON* type = cJSON_CreateString(c->type);
    cJSON_AddItemToObject(json, "type", type);

    cJSON* child;
    if (c->serialise != NULL) {child = c->serialise(c);}
    else {child = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "child", child);

    cJSON* positionOffset = cJSON_CreateArray();
    cJSON* xPositionOffset = cJSON_CreateNumber((double) c->positionOffset[0]);
    cJSON* yPositionOffset = cJSON_CreateNumber((double) c->positionOffset[1]);
    cJSON_AddItemToArray(positionOffset, xPositionOffset);
    cJSON_AddItemToArray(positionOffset, yPositionOffset);
    cJSON_AddItemToObject(json, "positionOffset", positionOffset);

    cJSON* sizeScale = cJSON_CreateArray();
    cJSON* xSizeScale = cJSON_CreateNumber((double) c->sizeScale[0]);
    cJSON* ySizeScale = cJSON_CreateNumber((double) c->sizeScale[1]);
    cJSON_AddItemToArray(sizeScale, xSizeScale);
    cJSON_AddItemToArray(sizeScale, ySizeScale);
    cJSON_AddItemToObject(json, "sizeScale", sizeScale);

    cJSON* rotationOffset = cJSON_CreateNumber((double) c->rotationOffset);
    cJSON_AddItemToObject(json, "rotationOffset", rotationOffset);

    return json;

}
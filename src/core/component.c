#include "entity.h"

Component* Component_Init(char* type, 
                          void (*update)(Component* c, float dt), 
                          void (*collision)(struct Component* c, struct Entity* with, vec2 contact, vec2 normal),
                          void (*free)(Component* c)) {

    Component* c = (Component*) malloc(sizeof(Component));

    // Store the type
    c->type = malloc(strlen(type)+1);
    memcpy(c->type, type, strlen(type)+1);

    glm_vec2_zero(c->positionOffset);
    glm_vec2_one(c->positionScale);
    glm_vec2_zero(c->sizeOffset);
    glm_vec2_one(c->sizeScale);
    c->rotationOffset = 0;
    c->rotationScale = 1; 

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

void Component_GetPosition(Component* c, vec2 dest) {
    glm_vec2_mul(c->entity->position, c->positionScale, dest);
    glm_vec2_add(dest, c->positionOffset, dest);
}

void Component_GetSize(Component* c, vec2 dest) {
    glm_vec2_mul(c->entity->size, c->sizeScale, dest);
    glm_vec2_add(dest, c->sizeOffset, dest);
}

float Component_GetRotation(Component* c) {
    return c->entity->rotation * c->rotationScale + c->rotationOffset;
}
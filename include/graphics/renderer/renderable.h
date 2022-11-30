#include "external.h"
#include "entity.h"
#include "wio.h"

#ifndef RENDERABLE_H
#define RENDERABLE_H

struct Renderable {

    Component* component;
    char* type;
    void* data;

    void (*update)(struct Renderable* r, float dt);
    void (*collision) (struct Renderable* r, Entity* with, vec2 contact, vec2 normal);
    cJSON* (*serialise) (struct Renderable* r);
    void (*free)(struct Renderable* r);

    vec4 colour;
    int zIndex;

    vec4 lastColour;
    int lastZIndex;
    vec2 lastPosition;
    vec2 lastSize;
    float lastRotation;

    bool isDirty;

};
typedef struct Renderable Renderable;

Component* Renderable_Init(char* type, vec4 colour, int zIndex);

bool Renderable_Load(Component* c, cJSON* json);

#endif
#include <stdbool.h>

#include "external.h"
#include "gameobject.h"
#include "sprite.h"
#include "transform.h"

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

struct SpriteRenderer {
    vec4 colour;
    Component* component;
    Sprite* sprite;
    Transform* transform;
    int zIndex;
    Transform lastTransform;
    bool isDirty;
};
typedef struct SpriteRenderer SpriteRenderer;

void SpriteRenderer_Init(Component* c, Sprite* sprite, vec4 colour, Transform* transform, int zIndex);

void SpriteRenderer_Update(Component* c, float dt);

void SpriteRenderer_Free(Component* c);

#endif
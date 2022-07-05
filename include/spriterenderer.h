#include <stdbool.h>

#include "external.h"
#include "gameobject.h"
#include "sprite.h"
#include "transform.h"

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

struct SpriteRenderer {
    vec4 colour;
    Component* component; // TODO: POINTER TO COMPONENT --> INDEX OF COMPONENT IN COMPONENT ARRAY
    Sprite* sprite;       // TODO: POINTER TO SPRITE --> INDEX OF SPRITE IN SPRITE ARRAY
    Transform* transform; // TODO: POINTER TO TRANSFORM --> ID OF TRANSFORM IN TRANSFORM ARRAY
    int zIndex;
    Transform lastTransform;
    bool isDirty;
};
typedef struct SpriteRenderer SpriteRenderer;

void SpriteRenderer_Init(Component* c, Sprite* sprite, vec4 colour, Transform* transform, int zIndex);

void SpriteRenderer_Update(Component* c, float dt);

void SpriteRenderer_Free(Component* c);

#endif
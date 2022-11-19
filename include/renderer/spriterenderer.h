#include <stdbool.h>

#include "external.h"
#include "gameobject.h"
#include "sprite.h"
#include "transform.h"

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

struct SpriteRenderer {

    Component* component;
    
    Sprite* sprite;
    vec4 colour;
    Transform* transform;
    int zIndex;

    Sprite lastSprite;
    Texture* lastTexture;
    vec4 lastColour;
    Transform lastTransform;
    int lastZIndex;

    bool isDirty;
};
typedef struct SpriteRenderer SpriteRenderer;

Component* SpriteRenderer_Init(Sprite* sprite, vec4 colour, Transform* transform, int zIndex);

void SpriteRenderer_Update(Component* c, float dt);

void SpriteRenderer_Free(Component* c);

#endif
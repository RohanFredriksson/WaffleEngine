#include <stdbool.h>

#include "external.h"
#include "gameobject.h"
#include "sprite.h"

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

struct SpriteRenderer {

    Component* component;
    
    Sprite* sprite;
    vec4 colour;
    int zIndex;

    Sprite lastSprite;
    Texture* lastTexture;
    vec4 lastColour;
    vec2 lastPosition;
    vec2 lastSize;
    float lastRotation;
    int lastZIndex;

    bool isDirty;
};
typedef struct SpriteRenderer SpriteRenderer;

Component* SpriteRenderer_Init(Sprite* sprite, vec4 colour, int zIndex);

void SpriteRenderer_Update(Component* c, float dt);

void SpriteRenderer_GetPosition(SpriteRenderer* s, vec2 dest);

void SpriteRenderer_GetSize(SpriteRenderer* s, vec2 dest);

float SpriteRenderer_GetRotation(SpriteRenderer* s);

#endif
#include <stdlib.h>
#include <stdbool.h>
#include "external.h"
#include "assetpool.h"
#include "entity.h"
#include "sprite.h"
#include "wio.h"

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

bool SpriteRenderer_Load(Component* c, cJSON* json);

#endif
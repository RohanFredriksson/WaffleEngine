#include <stdbool.h>

#include "external.h"
#include "sprite.h"
#include "transform.h"

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

struct SpriteRenderer {

    void (*update)(struct SpriteRenderer* s, float dt); // Component update method.

    Sprite* sprite;
    vec4 colour;
    Transform* transform;
    int zIndex;

    Transform lastTransform;
    bool isDirty;

};
typedef struct SpriteRenderer SpriteRenderer;

void SpriteRenderer_Init(SpriteRenderer* s, Sprite* sprite, vec4 colour, Transform* transform, int zIndex);

void SpriteRenderer_Update(SpriteRenderer* s, float dt);

#endif
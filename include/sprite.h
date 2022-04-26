#include "external.h"
#include "texture.h"

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite {
    Texture* texture;
    vec2 texCoords[4];
    vec2 size;    
};
typedef struct Sprite Sprite;

void Sprite_Init(Sprite* s, Texture* texture, vec2 texCoords[4], vec2 size);

#endif
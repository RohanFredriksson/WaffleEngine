#include <stdbool.h>
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

void Sprite_Init(Sprite* s, Texture* t);

void Sprite_SetTexCoords(Sprite* s, vec2 texCoords[4]);

void Sprite_SetSize(Sprite* s, vec2 size);

bool Sprite_Equals(Sprite s1, Sprite s2);

#endif
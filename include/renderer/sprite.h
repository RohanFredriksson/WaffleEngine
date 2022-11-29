#include <stdbool.h>
#include "external.h"
#include "texture.h"

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite {
    char* name;
    Texture* texture;
    vec2 texCoords[4];
};
typedef struct Sprite Sprite;

void Sprite_Init(Sprite* s, Texture* t, char* name);

void Sprite_Load(cJSON* json);

void Sprite_SetTexture(Sprite* s, Texture* texture);

void Sprite_SetTexCoords(Sprite* s, vec2 texCoords[4]);

void Sprite_SetName(Sprite* s, char* name);

bool Sprite_Equals(Sprite* s1, Sprite* s2);

cJSON* Sprite_Serialise(Sprite* s);

#endif
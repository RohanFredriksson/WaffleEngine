#include "external.h"
#include "texture.h"
#include "sprite.h"
#include "hashmap.h"

#ifndef FONT_H
#define FONT_H

struct CharInfo {

    int sourceX;
    int sourceY;
    int width;
    int height;

    vec2 texCoords[4];

};
typedef struct CharInfo CharInfo;

void CharInfo_CalculateTexCoords(CharInfo* info, int fontWidth, int fontHeight);

struct Font {
    HashMap sprites;
    HashMap info;
    Texture texture;
};
typedef struct Font Font;

void Font_Init(Font* font, char* filepath, int fontSize);

void Font_Free(Font* font);

Sprite* Font_Get(Font* font, int code);

CharInfo* Font_Info(Font* font, int code);

Texture* Font_GetTexture(Font* font);

#endif
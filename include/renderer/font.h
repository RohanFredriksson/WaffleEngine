#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stb_truetype.h"
#include "external.h"
#include "texture.h"
#include "sprite.h"
#include "hashmap.h"

#ifndef FONT_H
#define FONT_H

struct Font {
    char* filename;
    int size;
    Sprite* sprites;
    Texture texture;
    stbtt_fontinfo info;
};
typedef struct Font Font;

bool Font_Init(Font* font, char* filename, int size);

void Font_Free(Font* font);

Sprite* Font_Get(Font* font, char code);

float Font_GetScaleForHeight(Font* font, float height);

int Font_Advance(Font* font, char code);

int Font_Kerning(Font* font, char current, char next);

Texture* Font_GetTexture(Font* font);

#endif
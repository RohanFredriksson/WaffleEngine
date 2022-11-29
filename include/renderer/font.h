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
    HashMap sprites;
    Texture texture;
    stbtt_fontinfo info;
};
typedef struct Font Font;

void Font_Init(Font* font, char* filepath, int fontSize);

void Font_Free(Font* font);

/*
Sprite* Font_Get(Font* font, int code);

int Font_Kerning(Font* font, int current, int next);

Texture* Font_GetTexture(Font* font);
*/

#endif
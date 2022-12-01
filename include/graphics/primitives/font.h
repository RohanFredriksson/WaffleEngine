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
    float size;

    int ascent;
    int descent;
    int lineGap;
    int* advances;
    
    Sprite* sprites;
    Texture texture;

};
typedef struct Font Font;

bool Font_Init(Font* font, char* filename, float size);

cJSON* Font_Serialise(Font* font);

Font* Font_Parse(cJSON* json);

void Font_Load(cJSON* json);

void Font_Free(Font* font);

Sprite* Font_Get(Font* font, char code);

float Font_GetScaleForHeight(Font* font, float height);

int Font_Advance(Font* font, char code);

int Font_Ascent(Font* font);

int Font_Descent(Font* font);

int Font_LineGap(Font* font);

Texture* Font_GetTexture(Font* font);

bool Font_Equals(Font* a, Font* b);

#endif
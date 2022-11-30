#include "font.h"
#include "renderable.h"

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

struct TextRenderer {

    Renderable* renderable;
    char* text;
    Font* font;
    
    char* lastText;
    Font lastFont;
    
};
typedef struct TextRenderer TextRenderer;

Component* TextRenderer_Init(char* text, Font* font, vec4 colour, int zIndex);

bool TextRenderer_Load(Renderable* r, cJSON* json);

#endif
#include <stdbool.h>
#include "external.h"
#include "entity.h"
#include "font.h"

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

struct TextRenderer {

    Component* component;

    char* text;
    Font* font;
    vec4 colour;
    int zIndex;

    char* lastText;
    Font lastFont;
    vec4 lastColour;
    vec2 lastPosition;
    vec2 lastSize;
    float lastRotation;
    int lastZIndex;
    
    bool isDirty;
};
typedef struct TextRenderer TextRenderer;

Component* TextRenderer_Init(char* text, Font* font, vec4 colour, int zIndex);

bool TextRenderer_Load(Component* c, cJSON* json);

#endif
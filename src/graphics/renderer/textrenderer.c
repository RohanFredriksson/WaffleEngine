#include "stdlib.h"
#include "string.h"
#include "assetpool.h"
#include "textrenderer.h"

static void TextRenderer_Update(Component* c, float dt) {

    TextRenderer* t = (TextRenderer*) c->data;

    if (strcmp(t->text, t->lastText) != 0) {
        free(t->lastText);
        t->lastText = malloc(strlen(t->text) + 1);
        memcpy(t->lastText, t->text, strlen(t->text) + 1);
        t->isDirty = 1;
    }

    if (!Font_Equals(t->font, &t->lastFont)) {
        t->lastFont = *t->font;
        t->isDirty = 1;
    }

    if (!glm_vec4_eqv(t->colour, t->lastColour)) {
        glm_vec4_copy(t->colour, t->lastColour);
        t->isDirty = 1;
    }

    vec2 position;
    Component_GetPosition(c, position);
    if (!glm_vec2_eqv(position, t->lastPosition)) {
        glm_vec2_copy(position, t->lastPosition);
        t->isDirty = 1;
    }

    vec2 size;
    Component_GetSize(c, size);
    if (!glm_vec2_eqv(size, t->lastSize)) {
        glm_vec2_copy(size, t->lastSize);
        t->isDirty = 1;
    }

    float rotation = Component_GetRotation(c);
    if (rotation != t->lastRotation) {
        t->lastRotation = rotation;
        t->isDirty = 1;
    }

    if (t->zIndex != t->lastZIndex) {
        t->lastZIndex = t->zIndex;
        t->isDirty = 1;
    }

}

static cJSON* TextRenderer_Serialise(Component* c) {

    TextRenderer* t = (TextRenderer*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "text", t->text);
    WIO_AddString(json, "font", t->font->filename);
    WIO_AddVec4(json, "colour", t->colour);
    WIO_AddInt(json, "zIndex", t->zIndex);
    return json;

}

static void TextRenderer_Free(Component* c) {
    TextRenderer* t = (TextRenderer*) c->data;
    free(t->lastText);
}

TextRenderer* _TextRenderer_Init(Component* c, char* text, Font* font, vec4 colour, int zIndex) {

    TextRenderer* t = malloc(sizeof(TextRenderer));
    t->component = c;
    t->text = text;
    t->font = font;
    glm_vec4_copy(colour, t->colour);
    t->zIndex = zIndex;

    t->lastText = malloc(strlen(text) + 1);
    memcpy(t->lastText, text, strlen(text) + 1);
    t->lastFont = *font;
    glm_vec4_copy(colour, t->lastColour);
    glm_vec2_zero(t->lastPosition);
    glm_vec2_zero(t->lastSize);
    t->lastRotation = 0.0f;
    t->lastZIndex = zIndex;
    t->isDirty = 1;

    c->data = t;
    c->update = &TextRenderer_Update;
    c->serialise = &TextRenderer_Serialise;
    c->free = &TextRenderer_Free;

    return t;
}

Component* TextRenderer_Init(char* text, Font* font, vec4 colour, int zIndex) {
    Component* c = Component_Init("TextRenderer");
    _TextRenderer_Init(c, text, font, colour, zIndex);
    return c;
}

bool TextRenderer_Load(Component* c, cJSON* json) {

    char* text;
    char* fontName;
    vec4 colour;
    int zIndex;

    if (!WIO_ParseInt(json, "zIndex", &zIndex)) {return 0;}
    if (!WIO_ParseVec4(json, "colour", colour)) {return 0;}
    if (!WIO_ParseString(json, "text", &text)) {return 0;}
    if (!WIO_ParseString(json, "font", &fontName)) {return 0;}
    
    Font* font = FontPool_Get(fontName, 64);

    // Initialise the textrenderer class.
    _TextRenderer_Init(c, text, font, colour, zIndex);

    return 1;

}
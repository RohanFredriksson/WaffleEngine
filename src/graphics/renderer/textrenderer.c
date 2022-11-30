#include "stdlib.h"
#include "string.h"
#include "assetpool.h"
#include "textrenderer.h"

static void TextRenderer_Update(Renderable* r, float dt) {

    TextRenderer* t = (TextRenderer*) r->data;

    if (strcmp(t->text, t->lastText) != 0) {
        free(t->lastText);
        t->lastText = malloc(strlen(t->text) + 1);
        memcpy(t->lastText, t->text, strlen(t->text) + 1);
        r->isDirty = 1;
    }

    if (!Font_Equals(t->font, &t->lastFont)) {
        t->lastFont = *t->font;
        r->isDirty = 1;
    }

}

static cJSON* TextRenderer_Serialise(Renderable* r) {

    TextRenderer* t = (TextRenderer*) r->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "text", t->text);
    WIO_AddString(json, "font", t->font->filename);
    return json;

}

static void TextRenderer_Free(Renderable* r) {
    TextRenderer* t = (TextRenderer*) r->data;
    free(t->lastText);
}

TextRenderer* _TextRenderer_Init(Renderable* r, char* text, Font* font) {

    TextRenderer* t = malloc(sizeof(TextRenderer));
    t->renderable = r;
    t->text = text;
    t->font = font;

    t->lastText = malloc(strlen(text) + 1);
    memcpy(t->lastText, text, strlen(text) + 1);
    t->lastFont = *font;

    r->data = t;
    r->update = &TextRenderer_Update;
    r->serialise = &TextRenderer_Serialise;
    r->free = &TextRenderer_Free;

    return t;
}

Component* TextRenderer_Init(char* text, Font* font, vec4 colour, int zIndex) {
    Component* c = Renderable_Init("TextRenderer", colour, zIndex);
    _TextRenderer_Init((Renderable*) c->data, text, font);
    return c;
}

bool TextRenderer_Load(Renderable* r, cJSON* json) {

    char* text;
    char* fontName;

    if (!WIO_ParseString(json, "text", &text)) {return 0;}
    if (!WIO_ParseString(json, "font", &fontName)) {return 0;}
    Font* font = FontPool_Get(fontName, 64);

    // Initialise the textrenderer class.
    _TextRenderer_Init(r, text, font);

    return 1;

}
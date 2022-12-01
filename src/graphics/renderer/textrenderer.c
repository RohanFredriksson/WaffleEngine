#include "stdlib.h"
#include "string.h"
#include "window.h"
#include "scene.h"
#include "assetpool.h"
#include "textrenderer.h"

static void TextRenderer_ClearText(TextRenderer* t) {

    int n = List_Length(&t->components);
    for (int i = 0; i < n; i++) {
        int id;
        List_Get(&t->components, i, &id);
        Component* component = Entity_GetComponentByID(t->component->entity, id);
        if (component != NULL) {Component_Kill(component);}
    }
    List_Clear(&t->components);

}

static void TextRenderer_UpdateText(TextRenderer* t) {

    // Clear the old text that was there.
    TextRenderer_ClearText(t);

    vec2 size;
    Component_GetSize(t->component, size);

    vec2 halfSize;
    vec2 min;
    Component_GetPosition(t->component, min);
    glm_vec2_scale(size, 0.5f, halfSize);
    glm_vec2_sub(min, halfSize, min);

    float scale = Font_GetScaleForHeight(t->font, size[1]);

    float x = min[0];
    float y = min[1];

    int n = strlen(t->text);
    for (int i = 0; i < n; i++) {

        float width = scale * Font_Advance(t->font, t->text[i]);
        float height = size[1];

        if (t->text[i] == '\n') {
            x = min[0];
            y -= size[1];
        }

        else {

            if (t->text[i] != ' ') {

                Component* spriteRenderer = SpriteRenderer_Init(Font_Get(t->font, t->text[i]), t->colour, t->zIndex);

                vec2 offset;
                vec2 scale;
                glm_vec2_sub((vec2) {x + width * 0.5f, y + height * 0.5f}, t->component->entity->position, offset);
                glm_vec2_div((vec2) {width, height}, size, scale);

                Component_SetPositionOffset(spriteRenderer, offset);
                Component_SetSizeScale(spriteRenderer, scale);
                Component_SetRotationOffset(spriteRenderer, -t->component->entity->rotation);
                Component_Ignore(spriteRenderer); // Do not serialise this component.

                Entity_AddComponent(t->component->entity, spriteRenderer);
                List_Push(&t->components, &spriteRenderer->id);
            }
            
            x += width;
        }

    }
    t->isDirty = 0;
}

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

    // If the text has changed, update the text.
    if (t->isDirty) {TextRenderer_UpdateText(t);}

}

static cJSON* TextRenderer_Serialise(Component* c) {

    TextRenderer* t = (TextRenderer*) c->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "text", t->text);

    cJSON* font = Font_Serialise(t->font);
    cJSON_AddItemToObject(json, "font", font);

    WIO_AddVec4(json, "colour", t->colour);
    WIO_AddInt(json, "zIndex", t->zIndex);
    return json;

}

static void TextRenderer_Free(Component* c) {
    
    TextRenderer* t = (TextRenderer*) c->data;
    free(t->lastText);

    // Kill all the other components.
    int n = List_Length(&t->components);
    for (int i = 0; i < n; i++) {
        int id;
        List_Get(&t->components, i, &id);
        Component* component = Entity_GetComponentByID(t->component->entity, id);
        if (component != NULL) {Component_Kill(component);}
    }
    List_Free(&t->components);

}

TextRenderer* _TextRenderer_Init(Component* c, char* text, Font* font, vec4 colour, int zIndex) {

    TextRenderer* t = malloc(sizeof(TextRenderer));
    t->component = c;
    List_Init(&t->components, sizeof(int));
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
    Font* font;
    vec4 colour;
    int zIndex;

    if (!WIO_ParseInt(json, "zIndex", &zIndex)) {return 0;}
    if (!WIO_ParseVec4(json, "colour", colour)) {return 0;}
    if (!WIO_ParseString(json, "text", &text)) {return 0;}
    
    cJSON* f = cJSON_GetObjectItemCaseSensitive(json, "font");
    if (f == NULL) {return 0;}

    font = Font_Parse(f);
    if (font == NULL) {return 0;}

    // Initialise the textrenderer class.
    _TextRenderer_Init(c, text, font, colour, zIndex);

    return 1;
}
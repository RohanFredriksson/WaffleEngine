#include <stdlib.h>
#include "external.h"
#include "entity.h"
#include "spriterenderer.h"
#include "sprite.h"

Component* SpriteRenderer_Init(Sprite* sprite, vec4 colour, int zIndex) {
    
    Component* c = Component_Init("SpriteRenderer", &SpriteRenderer_Update, NULL, NULL);

    // Allocate some memory for the sprite renderer.
    SpriteRenderer* s = malloc(sizeof(SpriteRenderer));

    // Initialise the sprite renderer.
    s->component = c;

    s->sprite = sprite;
    glm_vec4_copy(colour, s->colour);
    s->zIndex = zIndex;

    s->lastSprite = *sprite;
    s->lastTexture = NULL;
    glm_vec4_copy(colour, s->lastColour);
    glm_vec2_zero(s->lastPosition);
    glm_vec2_zero(s->lastSize);
    s->lastRotation = 0.0f;
    s->lastZIndex = zIndex;

    s->isDirty = 1;

    // Attach the sprite renderer to the component
    c->data = s;
    return c;

}

void SpriteRenderer_Update(Component* c, float dt) {

    SpriteRenderer* s = (SpriteRenderer*) c->data;

    if (!Sprite_Equals(*s->sprite, s->lastSprite)) {

        // If the texture changed, keep a record of it.
        if (s->sprite->texture != s->lastSprite.texture) {
            s->lastTexture = s->lastSprite.texture;
        }

        s->lastSprite = *s->sprite;
        s->isDirty = 1;
    }

    if (!glm_vec4_eqv(s->colour, s->lastColour)) {
        glm_vec4_copy(s->colour, s->lastColour);
        s->isDirty = 1;
    }

    vec2 position;
    SpriteRenderer_GetPosition(s, position);
    if (!glm_vec2_eqv(position, s->lastPosition)) {
        glm_vec2_copy(position, s->lastPosition);
        s->isDirty = 1;
    }

    vec2 size;
    SpriteRenderer_GetSize(s, size);
    if (!glm_vec2_eqv(size, s->lastSize)) {
        glm_vec2_copy(size, s->lastSize);
        s->isDirty = 1;
    }

    float rotation = SpriteRenderer_GetRotation(s);
    if (rotation != s->lastRotation) {
        s->lastRotation = rotation;
        s->isDirty = 1;
    }

    if (s->zIndex != s->lastZIndex) {
        s->lastZIndex = s->zIndex;
        s->isDirty = 1;
    }

}

void SpriteRenderer_GetPosition(SpriteRenderer* s, vec2 dest) {
    glm_vec2_copy(s->component->entity->position, dest);
}

void SpriteRenderer_GetSize(SpriteRenderer* s, vec2 dest) {
    glm_vec2_copy(s->component->entity->size, dest);
}

float SpriteRenderer_GetRotation(SpriteRenderer* s) {
    return s->component->entity->rotation;
}
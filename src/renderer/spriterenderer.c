#include <stdlib.h>
#include "external.h"
#include "gameobject.h"
#include "spriterenderer.h"
#include "sprite.h"
#include "transform.h"

Component* SpriteRenderer_Init(Sprite* sprite, vec4 colour, Transform* transform, int zIndex) {
    
    Component* c = Component_Init("SpriteRenderer", &SpriteRenderer_Update, NULL, NULL);

    // Allocate some memory for the sprite renderer.
    SpriteRenderer* s = malloc(sizeof(SpriteRenderer));

    // Initialise the sprite renderer.
    s->component = c;

    s->sprite = sprite;
    glm_vec4_copy(colour, s->colour);
    s->transform = transform;
    s->zIndex = zIndex;

    s->lastSprite = *sprite;
    s->lastTexture = NULL;
    glm_vec4_copy(colour, s->lastColour);
    s->lastTransform = *transform;
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

    if (!Transform_Equals(*s->transform, s->lastTransform)) {
        s->lastTransform = *s->transform;
        s->isDirty = 1;
    }

    if (s->zIndex != s->lastZIndex) {
        s->lastZIndex = s->zIndex;
        s->isDirty = 1;
    }

}
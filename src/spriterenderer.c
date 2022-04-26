#include <stdlib.h>
#include "external.h"
#include "gameobject.h"
#include "spriterenderer.h"
#include "transform.h"

void SpriteRenderer_Init(Component* c, Sprite* sprite, vec4 colour, Transform* transform, int zIndex) {
    
    Component_Init(c, "SpriteRenderer", &SpriteRenderer_Update, &SpriteRenderer_Free);

    // Allocate some memory for the sprite renderer.
    SpriteRenderer* s = malloc(sizeof(SpriteRenderer));

    // Initialise the sprite renderer.
    s->component = c;
    s->sprite = sprite;
    glm_vec4_copy(colour, s->colour);
    s->transform = transform;
    s->zIndex = zIndex;
    s->lastTransform = *transform;
    s->isDirty = 1;

    // Attach the sprite renderer to the component
    c->data = s;

}

void SpriteRenderer_Update(Component* c, float dt) {

    SpriteRenderer* s = (SpriteRenderer*) c->data;
    if (!Transform_Equals(*s->transform, s->lastTransform)) {
        s->lastTransform = *s->transform;
        s->isDirty = 1;
    }

}

void SpriteRenderer_Free(Component* c) {
    Component_Free(c);
    free(c->data);
}
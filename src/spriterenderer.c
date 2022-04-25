#include "external.h"
#include "spriterenderer.h"
#include "transform.h"

void SpriteRenderer_Init(SpriteRenderer* s, Sprite* sprite, vec4 colour, Transform* transform, int zIndex) {
    
    // Set up the function pointer.
    s->update = &SpriteRenderer_Update;

    s->sprite = sprite;
    glm_vec4_copy(colour, s->colour);
    s->transform = transform;
    s->zIndex = zIndex;

    s->lastTransform = *transform;
    s->isDirty = 1;

}

void SpriteRenderer_Update(SpriteRenderer* s, float dt) {

    if (!Transform_Equals(*s->transform, s->lastTransform)) {
        s->lastTransform = *s->transform;
        s->isDirty = 1;
    }

}
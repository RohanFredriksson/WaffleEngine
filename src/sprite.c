#include <stdlib.h>
#include <string.h>
#include "sprite.h"

void Sprite_Init(Sprite* s, Texture* texture, vec2 texCoords[4], vec2 size) {
    s->texture = texture;
    glm_vec2_copy(size, s->size);
    memcpy(s->texCoords, texCoords, sizeof(s->texCoords));
}
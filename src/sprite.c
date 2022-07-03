#include <stdlib.h>
#include <string.h>
#include "sprite.h"
#include "texturepool.h"

void Sprite_Init(Sprite* s, const char* filename) {
    
    Texture* texture = TexturePool_Get(filename);
    s->texture = TexturePool_GetIndexOf(texture->filename);

    if (texture != NULL) {    
        s->size[0] = texture->width;
        s->size[1] = texture->height;
    }

    s->texCoords[0][0] = 1;
    s->texCoords[0][1] = 1;
    s->texCoords[1][0] = 1;
    s->texCoords[1][1] = 0;
    s->texCoords[2][0] = 0;
    s->texCoords[2][1] = 0;
    s->texCoords[3][0] = 0;
    s->texCoords[3][1] = 1;

}

void Sprite_SetTexCoords(Sprite* s, vec2 texCoords[4]) {
    memcpy(s->texCoords, texCoords, sizeof(s->texCoords));
}

void Sprite_SetSize(Sprite* s, vec2 size) {
    glm_vec2_copy(size, s->size);
}
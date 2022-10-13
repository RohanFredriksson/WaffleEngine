#include <stdlib.h>
#include <string.h>
#include "sprite.h"
#include "texture.h"
#include "texturepool.h"

void Sprite_Init(Sprite* s, Texture* t) {
    
    s->texture = t;
    if (s->texture != NULL) {    
        s->size[0] = s->texture->width;
        s->size[1] = s->texture->height;
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

bool Sprite_Equals(Sprite s1, Sprite s2) {

    // If the textures are not the same, return 0.
    if (s1.texture != s2.texture) {
        return 0;
    }
    
    // If the size is not the same, return 0.
    for (int i = 0; i < 2; i++) {
        if (s1.size[i] != s2.size[i]) {return 0;}
    }

    // If the texture coords are not the same, return 0.
    for (int i = 0; i < 4; i++) {
        if (s1.texCoords[i][0] != s2.texCoords[i][0]) {return 0;}
        if (s1.texCoords[i][1] != s2.texCoords[i][1]) {return 0;}
    }

    // If reached here, then the sprites are the same, return 1.
    return 1;

}
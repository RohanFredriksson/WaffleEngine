#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "sprite.h"
#include "spritepool.h"
#include "texturepool.h"

static HashMap SpritePool;

void SpritePool_Init() {
    HashMap_Init(&SpritePool, sizeof(uint64_t), sizeof(Sprite*));
}

void SpritePool_Clear() {
    HashMap_Clear(&SpritePool);
}

void SpritePool_Free() {
    HashMap_Free(&SpritePool);
}

Sprite* SpritePool_Get(const char* filename) {

    // Compute a string hash.
    uint64_t hash = OAAT(filename);
    Sprite* sprite;

    // If the sprite already exists, return the sprite.
    if (HashMap_Get(&SpritePool, &hash, &sprite)) {
        return sprite;
    }

    // Initialise the sprite.
    sprite = malloc(sizeof(Sprite));
    Sprite_Init(sprite, TexturePool_Get(filename));
    
    // Add the sprite to the sprite pool.
    HashMap_Put(&SpritePool, &hash, &sprite);

    // Return the new sprite.
    return sprite;

}
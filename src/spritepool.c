#include <stdlib.h>
#include <string.h>

#include "sprite.h"
#include "spritepool.h"
#include "texturepool.h"

#define INITIAL_SPRITEPOOL_SIZE 16

Sprite** SpritePool_Pool;
size_t SpritePool_Size;
size_t SpritePool_Length;
Sprite empty;

void SpritePool_Init() {
    
    // Initialise the sprite pool.
    SpritePool_Pool = (Sprite**) malloc(INITIAL_SPRITEPOOL_SIZE * sizeof(Sprite*));
    SpritePool_Size = INITIAL_SPRITEPOOL_SIZE;
    SpritePool_Length = 0;

    // Initialise the empty sprite.
    Sprite_Init(&empty, NULL);

}

void SpritePool_Clear() {

    // Free all sprite data.
    for (int i = 0; i < SpritePool_Length; i++) {
        free(SpritePool_Pool[i]);
    }

    // Set the length of the pool list to 0.
    SpritePool_Length = 0;

}

void SpritePool_Free() {
    
    // Free all sprite data.
    for (int i = 0; i < SpritePool_Length; i++) {
        free(SpritePool_Pool[i]);
    }

    // Free the pool.
    free(SpritePool_Pool);

}

Sprite* SpritePool_Get(const char* filename) {

    // If the filename is NULL, return the empty sprite.
    if (filename == NULL) {
        return &empty;
    }

    // If the sprite already exists, return the sprite.
    for (int i = 0; i < SpritePool_Length; i++) {
        Sprite* currentSprite = SpritePool_Pool[i];
        if (strcmp(currentSprite->texture->filename, filename) == 0) {
            return currentSprite;
        }
    }

    // The sprite does not exist. Add it.
    // If the pool is not big enough, allocate more memory.
    if (SpritePool_Length >= SpritePool_Size) {
        SpritePool_Pool = (Sprite**) realloc(SpritePool_Pool, SpritePool_Size * 2 * sizeof(Sprite*));
        SpritePool_Size = SpritePool_Size * 2;
    }

    // Initialise the sprite.
    Sprite* newSprite = (Sprite*) malloc(sizeof(Sprite));
    Sprite_Init(newSprite, TexturePool_Get(filename));
    SpritePool_Pool[SpritePool_Length] = newSprite;
    SpritePool_Length++;

    // Return the new sprite.
    return newSprite;

}
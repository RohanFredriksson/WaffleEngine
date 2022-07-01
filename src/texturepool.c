#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "texturepool.h"

#define INITIAL_TEXTUREPOOL_SIZE 16

Texture* pool;
size_t poolSize;
size_t poolLength;

void TexturePool_Init() {
    pool = (Texture*) malloc(INITIAL_TEXTUREPOOL_SIZE * sizeof(Texture));
    poolSize = INITIAL_TEXTUREPOOL_SIZE;
    poolLength = 0;
}

void TexturePool_Free() {
    
    // Free all texture data.
    for (int i = 0; i < poolLength; i++) {
        Texture_Free(pool + i);
    }

    // Free the pool.
    free(pool);

}

Texture* TexturePool_Get(const char* filename) {

    // If the texture already exists, return the texture.
    for (int i = 0; i < poolLength; i++) {
        Texture* currentTexture = pool + i;
        if (strcmp(currentTexture->filename, filename) == 0) {
            return currentTexture;
        }
    }

    // The texture does not exist. Add it.
    // If the pool is not big enough, allocate more memory.
    if (poolLength >= poolSize) {
        pool = (Texture*) realloc(pool, poolSize * 2 * sizeof(Texture));
        poolSize = poolSize * 2;
    }

    // Initialise the texture.
    Texture_Init(pool + poolLength, filename);
    poolLength++;

    // Return the new texture.
    return pool + (poolLength - 1);

}
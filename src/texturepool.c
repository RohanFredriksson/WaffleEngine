#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "texturepool.h"

#define INITIAL_TEXTUREPOOL_SIZE 16

Texture* texturePool;
size_t texturePoolSize;
size_t texturePoolLength;

void TexturePool_Init() {
    texturePool = (Texture*) malloc(INITIAL_TEXTUREPOOL_SIZE * sizeof(Texture));
    texturePoolSize = INITIAL_TEXTUREPOOL_SIZE;
    texturePoolLength = 0;
}

void TexturePool_Clear() {

    // Free all texture data.
    for (int i = 0; i < texturePoolLength; i++) {
        Texture_Free(texturePool + i);
    }

    // Set the length of the pool list to 0.
    texturePoolLength = 0;

}

void TexturePool_Free() {
    
    // Free all texture data.
    for (int i = 0; i < texturePoolLength; i++) {
        Texture_Free(texturePool + i);
    }

    // Free the pool.
    free(texturePool);

}

Texture* TexturePool_Get(const char* filename) {

    // If the texture already exists, return the texture.
    for (int i = 0; i < texturePoolLength; i++) {
        Texture* currentTexture = texturePool + i;
        if (strcmp(currentTexture->filename, filename) == 0) {
            return currentTexture;
        }
    }

    // The texture does not exist. Add it.
    // If the pool is not big enough, allocate more memory.
    if (texturePoolLength >= texturePoolSize) {
        texturePool = (Texture*) realloc(texturePool, texturePoolSize * 2 * sizeof(Texture));
        texturePoolSize = texturePoolSize * 2;
    }

    // Initialise the texture.
    Texture_Init(texturePool + texturePoolLength, filename);
    texturePoolLength++;

    // Return the new texture.
    return texturePool + (texturePoolLength - 1);

}

Texture* TexturePool_GetIndex(int index) {

    // If index out of range, return NULL.
    if (index < 0 || index >= texturePoolLength) {
        return NULL;
    }

    // Return the pointer to the texture.
    return texturePool + index;

}

int TexturePool_GetIndexOf(const char* filename) {

    // If the texture already exists, return the texture index.
    for (int i = 0; i < texturePoolLength; i++) {
        Texture* currentTexture = texturePool + i;
        if (strcmp(currentTexture->filename, filename) == 0) {
            return i;
        }
    }

    return -1;
}
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "texturepool.h"

#define INITIAL_TEXTUREPOOL_SIZE 16

Texture* TexturePool_Pool;
size_t TexturePool_Size;
size_t TexturePool_Length;

void TexturePool_Init() {
    TexturePool_Pool = (Texture*) malloc(INITIAL_TEXTUREPOOL_SIZE * sizeof(Texture));
    TexturePool_Size = INITIAL_TEXTUREPOOL_SIZE;
    TexturePool_Length = 0;
}

void TexturePool_Clear() {

    // Free all texture data.
    for (int i = 0; i < TexturePool_Length; i++) {
        Texture_Free(TexturePool_Pool + i);
    }

    // Set the length of the pool list to 0.
    TexturePool_Length = 0;

}

void TexturePool_Free() {
    
    // Free all texture data.
    for (int i = 0; i < TexturePool_Length; i++) {
        Texture_Free(TexturePool_Pool + i);
    }

    // Free the pool.
    free(TexturePool_Pool);

}

Texture* TexturePool_Get(const char* filename) {

    // If the texture already exists, return the texture.
    for (int i = 0; i < TexturePool_Length; i++) {
        Texture* currentTexture = TexturePool_Pool + i;
        if (strcmp(currentTexture->filename, filename) == 0) {
            return currentTexture;
        }
    }

    // The texture does not exist. Add it.
    // If the pool is not big enough, allocate more memory.
    if (TexturePool_Length >= TexturePool_Size) {
        TexturePool_Pool = (Texture*) realloc(TexturePool_Pool, TexturePool_Size * 2 * sizeof(Texture));
        TexturePool_Size = TexturePool_Size * 2;
    }

    // Initialise the texture.
    Texture_Init(TexturePool_Pool + TexturePool_Length, filename);
    TexturePool_Length++;

    // Return the new texture.
    return TexturePool_Pool + (TexturePool_Length - 1);

}

Texture* TexturePool_GetIndex(int index) {

    // If index out of range, return NULL.
    if (index < 0 || index >= TexturePool_Length) {
        return NULL;
    }

    // Return the pointer to the texture.
    return TexturePool_Pool + index;

}

int TexturePool_GetIndexOf(const char* filename) {

    // If the texture exists, return the texture index.
    for (int i = 0; i < TexturePool_Length; i++) {
        Texture* currentTexture = TexturePool_Pool + i;
        if (strcmp(currentTexture->filename, filename) == 0) {
            return i;
        }
    }
    
    // Else return -1.
    return -1;
    
}
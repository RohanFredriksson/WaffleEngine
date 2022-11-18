#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "texture.h"
#include "texturepool.h"

static HashMap TexturePool;

void TexturePool_Init() {
    HashMap_Init(&TexturePool, sizeof(uint64_t), sizeof(Texture*));
}

void TexturePool_Clear() {

    // Free all texture data.
    KeyValue* current = HashMap_Elements(&TexturePool);
    while (current != NULL) {
        Texture* texture = (Texture*) current->value;
        Texture_Free(texture);
        free(texture);
        current = current->next;
    }
    HashMap_Clear(&TexturePool);

}

void TexturePool_Free() {

    // Free all texture data.
    KeyValue* current = HashMap_Elements(&TexturePool);
    while (current != NULL) {
        Texture* texture = (Texture*) current->value;
        Texture_Free(texture);
        free(texture);
        current = current->next;
    }
    HashMap_Free(&TexturePool);

}

Texture* TexturePool_Get(const char* filename) {

    // Compute a string hash.
    uint64_t hash = OAAT(filename);
    Texture* texture;

    // If the texture already exists, return the texture.
    if (HashMap_Get(&TexturePool, &hash, &texture)) {
        return texture;
    }

    // Initialise the texture.
    texture = malloc(sizeof(Texture));
    Texture_Init(texture, filename);
    
    // Add the texture to the texture pool.
    HashMap_Put(&TexturePool, &hash, &texture);

    // Return the new texture.
    return texture;

}
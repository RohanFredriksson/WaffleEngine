#include "assetpool.h"

void AssetPool_Init() {
    ShaderPool_Init();
    TexturePool_Init();
    TransformPool_Init();
    SpritePool_Init();
}

void AssetPool_Free() {
    ShaderPool_Free();
    TexturePool_Free();
    TransformPool_Free();
    SpritePool_Free();
}

static HashMap ShaderPool;

struct ShaderHash {
    uint64_t vertex;
    uint64_t fragment;
};
typedef struct ShaderHash ShaderHash;

ShaderHash ShaderPool_Hash(const char* vertex, const char* fragment) {

    ShaderHash result;
    result.vertex = OAAT(vertex);
    result.fragment = OAAT(fragment);
    return result;

}

void ShaderPool_Init() {
    HashMap_Init(&ShaderPool, sizeof(ShaderHash), sizeof(Shader*));
}

void ShaderPool_Clear() {

    // Free all shader data.
    KeyValue* current = HashMap_Elements(&ShaderPool);
    while (current != NULL) {
        Shader* shader = *((Shader**) current->value);
        Shader_Free(shader);
        free(shader);
        current = current->next;
    }
    HashMap_Clear(&ShaderPool);

}

void ShaderPool_Free() {

    // Free all shader data.
    KeyValue* current = HashMap_Elements(&ShaderPool);
    while (current != NULL) {
        Shader* shader = *((Shader**) current->value);
        Shader_Free(shader);
        free(shader);
        current = current->next;
    }
    HashMap_Free(&ShaderPool);

}

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath) {

    // Create the ShaderHash object.
    ShaderHash hash = ShaderPool_Hash(vertexFilepath, fragmentFilepath);
    Shader* shader;

    // If the shader already exists, return the shader.
    if (HashMap_Get(&ShaderPool, &hash, &shader)) {
        return shader;
    }

    // Initialise and compile the shader.
    shader = malloc(sizeof(Shader));
    Shader_Init(shader, vertexFilepath, fragmentFilepath);
    Shader_Compile(shader);
    
    // Add the shader to the pool.
    HashMap_Put(&ShaderPool, &hash, &shader);

    // Return the new shader.
    return shader;

}

static HashMap SpritePool;

void SpritePool_Init() {
    HashMap_Init(&SpritePool, sizeof(uint64_t), sizeof(Sprite*));
}

void SpritePool_Clear() {

    // Free all sprite data.
    KeyValue* current = HashMap_Elements(&SpritePool);
    while (current != NULL) {
        Sprite* sprite = *((Sprite**) current->value);
        free(sprite);
        current = current->next;
    }
    HashMap_Clear(&SpritePool);

}

void SpritePool_Free() {

    // Free all sprite data.
    KeyValue* current = HashMap_Elements(&SpritePool);
    while (current != NULL) {
        Sprite* sprite = *((Sprite**) current->value);
        free(sprite);
        current = current->next;
    }
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

static HashMap TexturePool;

void TexturePool_Init() {
    HashMap_Init(&TexturePool, sizeof(uint64_t), sizeof(Texture*));
}

void TexturePool_Clear() {

    // Free all texture data.
    KeyValue* current = HashMap_Elements(&TexturePool);
    while (current != NULL) {
        Texture* texture = *((Texture**) current->value);
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
        Texture* texture = *((Texture**) current->value);
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

static List TransformPool;

void TransformPool_Init() {
    List_Init(&TransformPool, sizeof(Transform*));
}

void TransformPool_Clear() {

    // Free all transform data.
    Transform* transform;
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        List_Get(&TransformPool, i, &transform);
        free(transform);
    }
    List_Clear(&TransformPool);

}

void TransformPool_Free() {

    // Free all transform data.
    Transform* transform;
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        List_Get(&TransformPool, i, &transform);
        free(transform);
    }
    List_Free(&TransformPool);

}

Transform* TransformPool_Add(vec2 pos, vec2 size, float rotation) {

    // Initialise the transform.
    Transform* transform = (Transform*) malloc(sizeof(Transform));
    Transform_Init(transform, pos, size, rotation);

    // Push it on the list.
    List_Push(&TransformPool, &transform);

    // Return the new transform.
    return transform;

}

void TransformPool_Remove(Transform* t) {

    Transform* transform;
    int n = List_Length(&TransformPool);
    for (int i = 0; i < n; i++) {
        List_Get(&TransformPool, i, &transform);
        if (t == transform) {
            List_Remove(&TransformPool, i);
            break;
        }
    }

}

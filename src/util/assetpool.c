#include "assetpool.h"

void AssetPool_Init() {
    StringPool_Init();
    ShaderPool_Init();
    TexturePool_Init();
    SpritePool_Init();
    FontPool_Init();
}

void AssetPool_Free() {
    StringPool_Free();
    ShaderPool_Free();
    TexturePool_Free();
    SpritePool_Free();
    FontPool_Init();
}

static HashMap StringPool;

void StringPool_Init() {
    HashMap_Init(&StringPool, sizeof(uint64_t), sizeof(Texture*));
}

void StringPool_Clear() {

    // Free all string data.
    KeyValue* current = HashMap_Elements(&StringPool);
    while (current != NULL) {
        char* string = *((char**) current->value);
        free(string);
        current = current->next;
    }
    HashMap_Clear(&StringPool);

}

void StringPool_Free() {

    // Free all string data.
    KeyValue* current = HashMap_Elements(&StringPool);
    while (current != NULL) {
        char* string = *((char**) current->value);
        free(string);
        current = current->next;
    }
    HashMap_Free(&StringPool);

}

char* StringPool_Get(char* string) {

    // Compute a string hash.
    uint64_t hash = OAAT(string);
    char* result;

    // If the string already exists, return the string.
    if (HashMap_Get(&StringPool, &hash, &result)) {
        return result;
    }

    // Initialise the string.
    result = malloc(strlen(string) + 1);
    memcpy(result, string, strlen(string) + 1);
    
    // Add the string to the string pool.
    HashMap_Put(&StringPool, &hash, &result);

    // Return the new string.
    return result;

}

static HashMap ShaderPool;

struct ShaderHash {
    uint64_t vertex;
    uint64_t fragment;
};
typedef struct ShaderHash ShaderHash;

ShaderHash ShaderPool_Hash(char* vertex, char* fragment) {

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

Shader* ShaderPool_Get(char* vertexFilepath, char* fragmentFilepath) {

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

    // Put the empty sprite in the pool.
    Sprite* empty = malloc(sizeof(Sprite));
    Sprite_Init(empty, NULL, NULL);
    SpritePool_Put(empty);

    // Put the missing sprite in the pool.
    Sprite* missing = malloc(sizeof(Sprite));
    Sprite_Init(missing, TexturePool_Get("missing"), "missing");
    SpritePool_Put(missing);

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

cJSON* SpritePool_Serialise() {

    cJSON* json = cJSON_CreateArray();
    KeyValue* current = HashMap_Elements(&SpritePool);
    while (current != NULL) {
        cJSON* sprite = Sprite_Serialise(*((Sprite**) current->value));
        cJSON_AddItemToArray(json, sprite);
        current = current->next;
    }
    return json;

}

Sprite* SpritePool_Get(char* filename) {

    // Compute a string hash.
    uint64_t hash = OAAT(filename);
    Sprite* sprite;

    // If the sprite already exists, return the sprite.
    if (HashMap_Get(&SpritePool, &hash, &sprite)) {
        return sprite;
    }

    printf("ERROR::SPRITEPOOL_GET: sprite \"%s\" does not exist.\n", filename);

    // Return missing sprite.
    hash = OAAT("missing");
    HashMap_Get(&SpritePool, &hash, &sprite);
    return sprite;

}

void SpritePool_Put(Sprite* sprite) {
    uint64_t hash = OAAT(sprite->name);
    HashMap_Put(&SpritePool, &hash, &sprite);
}

bool SpritePool_Has(char* filename) {

    // Compute a string hash.
    uint64_t hash = OAAT(filename);
    Sprite* sprite;

    // If the sprite already exists.
    return HashMap_Get(&SpritePool, &hash, &sprite);

}

static HashMap TexturePool;

void TexturePool_Init() {
    
    HashMap_Init(&TexturePool, sizeof(uint64_t), sizeof(Texture*));

    // Create the missing texture.
    Texture* missing = malloc(sizeof(Texture));
    Texture_Init_Missing(missing);
    TexturePool_Put(missing);

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

Texture* TexturePool_Get(char* filename) {

    // Compute a string hash.
    uint64_t hash = OAAT(filename);
    Texture* texture;

    // If the texture already exists, return the texture.
    if (HashMap_Get(&TexturePool, &hash, &texture)) {
        return texture;
    }

    // Try to initialise the texture.
    texture = malloc(sizeof(Texture));
    if (!Texture_Init(texture, filename)) {
        free(texture);
        return NULL;
    }
    
    // Add the texture to the texture pool.
    HashMap_Put(&TexturePool, &hash, &texture);

    // Return the new texture.
    return texture;

}

void TexturePool_Put(Texture* texture) {
    uint64_t hash = OAAT(texture->filename);
    HashMap_Put(&TexturePool, &hash, &texture);
}

static HashMap FontPool;

struct FontHash {
    uint64_t filename;
    double size;
};
typedef struct FontHash FontHash;

FontHash FontPool_Hash(char* filename, float size) {

    FontHash result;
    result.filename = OAAT(filename);
    result.size = (double) size;
    return result;

}

void FontPool_Init() {
    HashMap_Init(&FontPool, sizeof(FontHash), sizeof(Font*));
}

void FontPool_Clear() {

    // Free all texture data.
    KeyValue* current = HashMap_Elements(&FontPool);
    while (current != NULL) {
        Font* font = *((Font**) current->value);
        Font_Free(font);
        free(font);
        current = current->next;
    }
    HashMap_Clear(&FontPool);

}
void FontPool_Free() {

    // Free all texture data.
    KeyValue* current = HashMap_Elements(&FontPool);
    while (current != NULL) {
        Font* font = *((Font**) current->value);
        Font_Free(font);
        free(font);
        current = current->next;
    }
    HashMap_Free(&FontPool);

}

cJSON* FontPool_Serialise() {

    cJSON* json = cJSON_CreateArray();
    KeyValue* current = HashMap_Elements(&FontPool);
    while (current != NULL) {
        cJSON* font = Font_Serialise(*((Font**) current->value));
        cJSON_AddItemToArray(json, font);
        current = current->next;
    }
    return json;

}

Font* FontPool_Get(char* filename, float size) {

    // Create the ShaderHash object.
    FontHash hash = FontPool_Hash(filename, size);
    Font* font;

    // If the font already exists, return the font.
    if (HashMap_Get(&FontPool, &hash, &font)) {
        return font;
    }

    // Initialise the font.
    font = malloc(sizeof(Font));
    if (!Font_Init(font, filename, size)) {
        free(font);
        return NULL;
    }
    
    // Add the font to the pool.
    HashMap_Put(&FontPool, &hash, &font);

    // Return the new font.
    return font;

}
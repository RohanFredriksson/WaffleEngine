#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
#include "shader.h"
#include "shaderpool.h"

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
        Shader* shader = (Shader*) current->value;
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
        Shader* shader = (Shader*) current->value;
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
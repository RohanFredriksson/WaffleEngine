#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "shaderpool.h"

#define INITIAL_SHADERPOOL_SIZE 16

Shader* shaderPool;
size_t shaderPoolSize;
size_t shaderPoolLength;

void ShaderPool_Init() {
    shaderPool = (Shader*) malloc(INITIAL_SHADERPOOL_SIZE * sizeof(Shader));
    shaderPoolSize = INITIAL_SHADERPOOL_SIZE;
    shaderPoolLength = 0;
}

void ShaderPool_Clear() {

    // Free all shader data.
    for (int i = 0; i < shaderPoolLength; i++) {
        Shader_Free(shaderPool + i);
    }

    // Set the length of the shader pool list to 0.
    shaderPoolLength = 0;

}

void ShaderPool_Free() {
    
    // Free all shader data.
    for (int i = 0; i < shaderPoolLength; i++) {
        Shader_Free(shaderPool + i);
    }

    // Free the shader pool.
    free(shaderPool);

}

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath) {

    // If the shader already exists, return the shader.
    for (int i = 0; i < shaderPoolLength; i++) {
        Shader* currentShader = shaderPool + i;
        if (strcmp(currentShader->vertexFilepath, vertexFilepath) == 0 && 
            strcmp(currentShader->fragmentFilepath, fragmentFilepath) == 0) {
            return currentShader;
        }
    }

    // The shader does not exist. Add it.
    // If the shaderPool is not big enough, allocate more memory.
    if (shaderPoolLength >= shaderPoolSize) {
        shaderPool = (Shader*) realloc(shaderPool, shaderPoolSize * 2 * sizeof(Shader));
        shaderPoolSize = shaderPoolSize * 2;
    }

    // Initialise and compile the shader.
    Shader* shader = shaderPool + shaderPoolLength;
    Shader_Init(shader, vertexFilepath, fragmentFilepath);
    Shader_Compile(shader);
    shaderPoolLength++;

    // Return the new shader.
    return shader;

}
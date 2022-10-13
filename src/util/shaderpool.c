#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "shaderpool.h"

#define INITIAL_SHADERPOOL_SIZE 16

Shader** ShaderPool_Pool;
size_t ShaderPool_Size;
size_t ShaderPool_Length;

void ShaderPool_Init() {
    ShaderPool_Pool = (Shader**) malloc(INITIAL_SHADERPOOL_SIZE * sizeof(Shader*));
    ShaderPool_Size = INITIAL_SHADERPOOL_SIZE;
    ShaderPool_Length = 0;
}

void ShaderPool_Clear() {

    // Free all shader data.
    for (int i = 0; i < ShaderPool_Length; i++) {
        Shader_Free(ShaderPool_Pool[i]);
        free(ShaderPool_Pool[i]);
    }

    // Set the length of the shader pool list to 0.
    ShaderPool_Length = 0;

}

void ShaderPool_Free() {
    
    // Free all shader data.
    for (int i = 0; i < ShaderPool_Length; i++) {
        Shader_Free(ShaderPool_Pool[i]);
        free(ShaderPool_Pool[i]);
    }

    // Free the shader pool.
    free(ShaderPool_Pool);

}

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath) {

    // If the shader already exists, return the shader.
    for (int i = 0; i < ShaderPool_Length; i++) {
        Shader* currentShader = ShaderPool_Pool[i];
        if (strcmp(currentShader->vertexFilepath, vertexFilepath) == 0 && 
            strcmp(currentShader->fragmentFilepath, fragmentFilepath) == 0) {
            return currentShader;
        }
    }

    // The shader does not exist. Add it.
    // If the shaderpool is not big enough, allocate more memory.
    if (ShaderPool_Length >= ShaderPool_Size) {
        ShaderPool_Pool = (Shader**) realloc(ShaderPool_Pool, ShaderPool_Size * 2 * sizeof(Shader*));
        ShaderPool_Size = ShaderPool_Size * 2;
    }

    // Initialise and compile the shader.
    Shader* newShader = (Shader*) malloc(sizeof(Shader));
    Shader_Init(newShader, vertexFilepath, fragmentFilepath);
    Shader_Compile(newShader);
    ShaderPool_Pool[ShaderPool_Length] = newShader;
    ShaderPool_Length++;

    // Return the new shader.
    return newShader;

}
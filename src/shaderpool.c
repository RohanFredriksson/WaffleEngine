#include <stdlib.h>
#include <string.h>

#include "shader.h"
#include "shaderpool.h"

#define INITIAL_SHADERPOOL_SIZE 16

Shader* pool;
size_t poolSize;
size_t poolLength;

void ShaderPool_Init() {
    pool = (Shader*) malloc(INITIAL_SHADERPOOL_SIZE * sizeof(Shader));
    poolSize = INITIAL_SHADERPOOL_SIZE;
    poolLength = 0;
}

void ShaderPool_Clear() {

    // Free all shader data.
    for (int i = 0; i < poolLength; i++) {
        Shader_Free(pool + i);
    }

    // Set the length of the pool list to 0.
    poolLength = 0;

}

void ShaderPool_Free() {
    
    // Free all shader data.
    for (int i = 0; i < poolLength; i++) {
        Shader_Free(pool + i);
    }

    // Free the pool.
    free(pool);

}

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath) {

    // If the shader already exists, return the shader.
    for (int i = 0; i < poolLength; i++) {
        Shader* currentShader = pool + i;
        if (strcmp(currentShader->vertexFilepath, vertexFilepath) == 0 && 
            strcmp(currentShader->fragmentFilepath, fragmentFilepath) == 0) {
            return currentShader;
        }
    }

    // The shader does not exist. Add it.
    // If the pool is not big enough, allocate more memory.
    if (poolLength >= poolSize) {
        pool = (Shader*) realloc(pool, poolSize * 2 * sizeof(Shader));
        poolSize = poolSize * 2;
    }

    // Initialise and compile the shader.
    Shader_Init(pool + poolLength, vertexFilepath, fragmentFilepath);
    Shader_Compile(pool + poolLength);
    poolLength++;

    // Return the new shader.
    return pool + (poolLength - 1);

}
#include <stdlib.h>
#include <string.h>
#include "external.h"

#include "shader.h"
#include "shaderpool.h"

List ShaderPool_Pool;

void ShaderPool_Init() {
    List_Init(&ShaderPool_Pool, sizeof(Shader*));
}

void ShaderPool_Clear() {
    List_Clear(&ShaderPool_Pool);
}

void ShaderPool_Free() {
    List_Free(&ShaderPool_Pool);
}

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath) {

    // If the shader already exists, return the shader.
    for (int i = 0; i < List_Length(&ShaderPool_Pool); i++) {
        
        Shader* current;
        List_Get(&ShaderPool_Pool, i, &current);

        if (strcmp(current->vertexFilepath, vertexFilepath) == 0 && 
            strcmp(current->fragmentFilepath, fragmentFilepath) == 0) {
            return current;
        }

    }

    // The shader does not exist. Create the shader.
    Shader* new = (Shader*) malloc(sizeof(Shader));
    Shader_Init(new, vertexFilepath, fragmentFilepath);
    Shader_Compile(new);

    // Add it to the list
    List_Push(&ShaderPool_Pool, &new);

    // Return the new shader.
    return new;
}
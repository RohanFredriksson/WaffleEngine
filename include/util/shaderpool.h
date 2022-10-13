#include "shader.h"

#ifndef SHADERPOOL_H
#define SHADERPOOL_H

void ShaderPool_Init();
void ShaderPool_Clear();
void ShaderPool_Free();

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath);

#endif
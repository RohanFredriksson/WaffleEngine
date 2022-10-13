#include <stdbool.h>
#include "external.h"

#ifndef SHADER_H
#define SHADER_H

struct Shader {
    GLuint program;
    bool beingUsed;
    char* vertexFilepath;
    char* fragmentFilepath;
    char* vertexCode;
    char* fragmentCode;
};
typedef struct Shader Shader;

void Shader_Init(Shader* s, const char* vertexFilepath, const char* fragmentFilepath);
void Shader_Compile(Shader* s);
void Shader_Use(Shader* s);
void Shader_Detach(Shader* s);
void Shader_Free(Shader* s);
char* Shader_LoadSource(const char* filepath);

void Shader_UploadFloat(Shader* s, const char* name, float value);
void Shader_UploadVec2(Shader* s, const char* name, vec2 vector);
void Shader_UploadVec3(Shader* s, const char* name, vec3 vector);
void Shader_UploadVec4(Shader* s, const char* name, vec4 vector);
void Shader_UploadMat4(Shader* s, const char* name, mat4 matrix);
void Shader_UploadTexture(Shader* s, const char* name, int slot);
void Shader_UploadIntArray(Shader* s, const char* name, int num, int* array);

#endif
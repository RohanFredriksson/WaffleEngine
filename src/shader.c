#include <stdio.h>
#include <string.h>

#include "external.h"
#include "shader.h"

void Shader_Init(Shader* s, const char* vertexFilepath, const char* fragmentFilepath) {
    
    s->vertexFilepath = malloc(strlen(vertexFilepath));
    s->fragmentFilepath = malloc(strlen(fragmentFilepath));
    memcpy(s->vertexFilepath, vertexFilepath, strlen(vertexFilepath));
    memcpy(s->fragmentFilepath, fragmentFilepath, strlen(fragmentFilepath));
    s->vertexCode = Shader_LoadSource(vertexFilepath);
    s->fragmentCode = Shader_LoadSource(fragmentFilepath);
    s->beingUsed = 0;

}

void Shader_Compile(Shader* s) {

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader Compilation.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (char const * const *)&(s->vertexCode), NULL);
    glCompileShader(vertex);

    // Check for compile errors.
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    // Fragment Shader Compilation.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (char const * const *)&(s->fragmentCode), NULL);
    glCompileShader(fragment);

    // Check for compile errors.
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    // Shader Program Linking.
    s->program = glCreateProgram();
    glAttachShader(s->program, vertex);
    glAttachShader(s->program, fragment);
    glLinkProgram(s->program);

    // Check if any errors occured in linking.
    glGetProgramiv(s->program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(s->program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader_Use(Shader* s) {
    if (!s->beingUsed) {
        glUseProgram(s->program);
        s->beingUsed = true;
    }
}

void Shader_Detach(Shader* s) {
    glUseProgram(0);
    s->beingUsed = false;
}

void Shader_Free(Shader* s) {
    glDeleteProgram(s->program);
    free(s->vertexFilepath);
    free(s->fragmentFilepath);
    free(s->vertexCode);
    free(s->fragmentCode);
}

char* Shader_LoadSource(const char* filepath) {
    FILE* f = fopen(filepath, "r");
    fseek(f, 0L, SEEK_END);
    size_t size = ftell(f) + 1;
    char* source = malloc(size);
    rewind(f);
    fread(source, size, sizeof(char), f);
    fclose(f);
    source[size-1] = '\0';
    return source;   
}

void Shader_UploadFloat(Shader* s, const char* name, float value) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform1f(location, value);
}

void Shader_UploadVec2(Shader* s, const char* name, vec2 vector) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform2f(location, vector[0], vector[1]);
}

void Shader_UploadVec3(Shader* s, const char* name, vec3 vector) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform3f(location, vector[0], vector[1], vector[2]);
}

void Shader_UploadVec4(Shader* s, const char* name, vec4 vector) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform4f(location, vector[0], vector[1], vector[2], vector[3]);
}

void Shader_UploadMat4(Shader* s, const GLchar* name, mat4 matrix) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix[0]);
}

void Shader_UploadTexture(Shader* s, const char* name, int slot) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform1i(location, slot);
}

void Shader_UploadIntArray(Shader* s, const char* name, int num, int* array) {
    GLint location = glGetUniformLocation(s->program, name);
    Shader_Use(s);
    glUniform1iv(location, num, array);
}
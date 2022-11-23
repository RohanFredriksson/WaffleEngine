#include "shader.h"

void Shader_Init(Shader* s, char* vertexFilepath, char* fragmentFilepath) {
    s->vertexFilepath = vertexFilepath;
    s->fragmentFilepath = fragmentFilepath;
    s->beingUsed = 0;
}

void Shader_Compile(Shader* s) {

    char* vertexCode = WIO_LoadSource(s->vertexFilepath);
    char* fragmentCode = WIO_LoadSource(s->fragmentFilepath);
    if (vertexCode == NULL) {printf("ERROR::SHADER::INIT::VERTEX_SOURCE_NOT_FOUND\n");}
    if (fragmentCode == NULL) {printf("ERROR::SHADER::INIT::FRAGMENT_SOURCE_NOT_FOUND\n");}

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader Compilation.
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (char const * const *)&(vertexCode), NULL);
    glCompileShader(vertex);

    // Check for compile errors.
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    // Fragment Shader Compilation.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (char const * const *)&(fragmentCode), NULL);
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

    free(vertexCode);
    free(fragmentCode);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader_Use(Shader* s) {
    if (!s->beingUsed) {
        glUseProgram(s->program);
        s->beingUsed = 1;
    }
}

void Shader_Detach(Shader* s) {
    glUseProgram(0);
    s->beingUsed = 0;
}

void Shader_Free(Shader* s) {
    glDeleteProgram(s->program);
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
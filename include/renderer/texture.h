#include <stdbool.h>

#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    char* filename;
    unsigned int id;
    int width, height, channels;
};
typedef struct Texture Texture;

bool Texture_Init(Texture* t, char* filename);
void Texture_New(Texture* t, int width, int height);
void Texture_New_RGB32F(Texture* t, int width, int height);
void Texture_Init_Missing(Texture* t);
void Texture_Save(Texture* t, char* filename);
void Texture_Bind(Texture* t);
void Texture_Unbind(Texture* t);
void Texture_Free(Texture* t);

#endif
#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    char* filename;
    unsigned int id;
    int width, height, channels;
};
typedef struct Texture Texture;

void Texture_Init(Texture* t, const char* filename);
void Texture_New(Texture* t, int width, int height);
void Texture_New_RGB32F(Texture* t, int width, int height);
void Texture_Save(Texture* t, const char* filename);
void Texture_Bind(Texture* t);
void Texture_Unbind(Texture* t);
void Texture_Free(Texture* t);

#endif
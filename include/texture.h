#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    char* filename;
    int id;
    int width, height, channels;
};
typedef struct Texture Texture;

void Texture_Init(Texture* t, const char* filename);
void Texture_Bind(Texture* t);
void Texture_Unbind(Texture* t);
void Texture_Free(Texture* t);

#endif
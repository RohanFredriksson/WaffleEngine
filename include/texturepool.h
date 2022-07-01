#include "texture.h"

#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

void TexturePool_Init();
void TexturePool_Free();
Texture* TexturePool_Get(const char* filename);

#endif
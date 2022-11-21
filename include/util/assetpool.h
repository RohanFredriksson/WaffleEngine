#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "sprite.h"
#include "texture.h"
#include "hashmap.h"
#include "hash.h"
#include "list.h"

#ifndef ASSETPOOL_H
#define ASSETPOOL_H

void AssetPool_Init();
void AssetPool_Free();

void ShaderPool_Init();
void ShaderPool_Clear();
void ShaderPool_Free();

Shader* ShaderPool_Get(const char* vertexFilepath, const char* fragmentFilepath);

void SpritePool_Init();
void SpritePool_Clear();
void SpritePool_Free();

Sprite* SpritePool_Get(const char* filename);
void SpritePool_Put(const char* name, Sprite* sprite);

void TexturePool_Init();
void TexturePool_Clear();
void TexturePool_Free();

Texture* TexturePool_Get(const char* filename);

#endif
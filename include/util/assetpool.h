#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "external.h"
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

void StringPool_Init();
void StringPool_Clear();
void StringPool_Free();

char* StringPool_Get(char* string);

void ShaderPool_Init();
void ShaderPool_Clear();
void ShaderPool_Free();

Shader* ShaderPool_Get(char* vertexFilepath, char* fragmentFilepath);

void SpritePool_Init();
void SpritePool_Clear();
void SpritePool_Free();
cJSON* SpritePool_Serialise();

Sprite* SpritePool_Get(char* filename);
void SpritePool_Put(Sprite* sprite);
bool SpritePool_Has(char* filename);

void TexturePool_Init();
void TexturePool_Clear();
void TexturePool_Free();

Texture* TexturePool_Get(char* filename);
void TexturePool_Put(Texture* texture);

#endif
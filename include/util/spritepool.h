#include "sprite.h"

#ifndef SPRITEPOOL_H
#define SPRITEPOOL_H

void SpritePool_Init();
void SpritePool_Clear();
void SpritePool_Free();

Sprite* SpritePool_Get(const char* filename);

#endif
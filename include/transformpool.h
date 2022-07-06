#include "transform.h"

#ifndef TRANSFORMPOOL_H
#define TRANSFORMPOOL_H

void TransformPool_Init();
void TransformPool_Clear();
void TransformPool_Free();

Transform* TransformPool_Get(int id);
Transform* TransformPool_Add(vec2 pos, vec2 size, float rotation);
void TransformPool_Remove(int id);

#endif
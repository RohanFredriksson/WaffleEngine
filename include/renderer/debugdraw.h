#include "external.h"

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

void DebugDraw_Init();
void DebugDraw_Start();
void DebugDraw_BeginFrame();
void DebugDraw_Draw();
void DebugDraw_Free();

void DebugDraw_AddLine2D(vec2 from, vec2 to, vec3 colour, int lifetime);
void DebugDraw_AddBox2D(vec2 centre, vec2 dimensions, float rotation, vec3 colour, int lifetime);
void DebugDraw_AddCircle(vec2 centre, float radius, vec3 colour, int lifetime);

#endif
#include <stdbool.h>
#include "entity.h"
#include "spriterenderer.h"
#include "texture.h"
#include "shader.h"

#ifndef RENDERER_H
#define RENDERER_H

struct Renderer {
    struct RenderBatch* batches;
    int numBatches;
    int sizeBatches;
};
typedef struct Renderer Renderer;

void Renderer_Init(Renderer* r);
void Renderer_AddComponent(Renderer* r, Component* component);
void Renderer_RemoveComponent(Renderer* r, Component* component);
void Renderer_BindShader(Shader* s);
Shader* Renderer_GetBoundShader();
void Renderer_Render(Renderer* r);
void Renderer_Free(Renderer* r);

struct RenderBatch {
    Renderer* renderer;
    SpriteRenderer** sprites;
    int numSprites;
    int sizeSprites;
    Texture** textures;
    int numTextures;
    bool hasRoom;
    bool hasTextureRoom;
    float* vertices;
    unsigned int vao;
    unsigned int vbo;
    int zIndex;
};
typedef struct RenderBatch RenderBatch;

#endif
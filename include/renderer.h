#include <stdbool.h>
#include "gameobject.h"
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

void Renderer_AddGameObject(Renderer* r, GameObject* go);

void Renderer_AddSprite(Renderer* r, SpriteRenderer* s);

void Renderer_RemoveGameObject(Renderer* r, GameObject* go);

void Renderer_RemoveSprite(Renderer* r, SpriteRenderer* s);

void Renderer_BindShader(Shader* s);

Shader* Renderer_GetBoundShader();

void Renderer_Render(Renderer* r);

void Renderer_Free(Renderer* r);

struct RenderBatch {
    Renderer* renderer;
    SpriteRenderer** sprites; // TODO: ARRAY OF POINTERS TO SPRITERENDERERS --> ARRAY OF INDICES OF SPRITERENDERERS IN SPRITERENDERERS ARRAY
    int numSprites;
    int sizeSprites;
    int* textures; // An array of indices of textures in the texture pool array.
    int numTextures;
    bool hasRoom;
    bool hasTextureRoom;
    float* vertices;
    int vao;
    int vbo;
    int zIndex;
};
typedef struct RenderBatch RenderBatch;

void RenderBatch_Init(RenderBatch* r, Renderer* renderer, int zIndex);

void RenderBatch_AddGameObject(RenderBatch* r, GameObject* go);

void RenderBatch_AddSprite(RenderBatch* r, SpriteRenderer* s);

void RenderBatch_Render(RenderBatch* r);

void RenderBatch_RemoveGameObject(RenderBatch* r, GameObject* go);

void RenderBatch_RemoveSprite(RenderBatch* r, SpriteRenderer* s);

void RenderBatch_LoadVertexProperties(RenderBatch* r, int index);

void RenderBatch_GenerateIndices(int* elements);

void RenderBatch_LoadElementIndices(int* elements, int index);

bool RenderBatch_HasRoom(RenderBatch* r);

bool RenderBatch_HasTextureRoom(RenderBatch* r);

bool RenderBatch_HasTexture(RenderBatch* r, int t);

void RenderBatch_AddTexture(RenderBatch* r, int t);

int RenderBatch_Compare(const void* a, const void* b);

void RenderBatch_Free(RenderBatch* r);

#endif
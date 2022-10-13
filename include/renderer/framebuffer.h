#include "texture.h"

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct FrameBuffer {
    int fbo;
    Texture texture;
};
typedef struct FrameBuffer FrameBuffer;

void FrameBuffer_Init(FrameBuffer* f, int width, int height);
void PickingTexture_Init(FrameBuffer* f, int width, int height);
void FrameBuffer_Bind(FrameBuffer* f);
void FrameBuffer_Unbind(FrameBuffer* f);
void FrameBuffer_Free(FrameBuffer* f);

#endif
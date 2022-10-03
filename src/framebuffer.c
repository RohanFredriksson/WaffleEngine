#include <stdio.h>

#include "external.h"
#include "framebuffer.h"
#include "texture.h"

void FrameBuffer_Init(FrameBuffer* f, int width, int height) {

    // Generate the framebuffer.
    glGenFramebuffers(1, &f->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo);

    // Initialise the texture to render the data to, and attach it to the framebuffer.
    Texture_New(&f->texture, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, f->texture.id, 0);

    // Create a renderbuffer.
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("ERROR::FRAMEBUFFER::FRAMEBUFFER_NOT_COMPLETE\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer_Bind(FrameBuffer* f) {
    glBindFramebuffer(GL_FRAMEBUFFER, f->fbo);
}

void FrameBuffer_Unbind(FrameBuffer* f) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer_Free(FrameBuffer* f) {
    Texture_Free(&f->texture);
}
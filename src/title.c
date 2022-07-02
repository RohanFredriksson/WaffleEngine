#include "title.h"
#include "renderer.h"
#include "texturepool.h"
#include "shaderpool.h"
#include "texture.h"
#include "sprite.h"
#include "gameobject.h"
#include "spriterenderer.h"
#include "transform.h"
#include "shader.h"
#include "listener.h"

void Title_Init(Scene* s) {
    printf("RENDERER BOUND\n");
    Renderer_BindShader(ShaderPool_Get("./assets/shaders/default.vert", "./assets/shaders/default.frag"));
}
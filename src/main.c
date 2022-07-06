#include "window.h"
#include "texturepool.h"
#include "shaderpool.h"
#include "transformpool.h"

int main() {

    TexturePool_Init();
    ShaderPool_Init();
    TransformPool_Init();

    Window_Run();

    TexturePool_Free();
    ShaderPool_Free();
    TransformPool_Free();
    
    return 0;
}
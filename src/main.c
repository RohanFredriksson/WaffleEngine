#include "window.h"
#include "texturepool.h"
#include "shaderpool.h"

int main() {

    TexturePool_Init();
    ShaderPool_Init();

    Window_Run();

    TexturePool_Free();
    ShaderPool_Free();
    
    return 0;
}
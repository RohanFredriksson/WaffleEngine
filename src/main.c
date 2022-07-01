#include "window.h"
#include "texturepool.h"

int main() {
    TexturePool_Init();
    Window_Run();
    TexturePool_Free();
    return 0;
}
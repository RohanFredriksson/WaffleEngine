#include "window.h"
#include "assetpool.h"

int main() {

    AssetPool_Init();
    Window_Run();
    AssetPool_Free();
    
    return 0;
}
#include "assetpool.h"

void AssetPool_Init() {
    ShaderPool_Init();
    TexturePool_Init();
    TransformPool_Init();
}

void AssetPool_Free() {
    ShaderPool_Free();
    TexturePool_Free();
    TransformPool_Free();
}
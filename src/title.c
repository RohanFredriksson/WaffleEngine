#include "title.h"
#include "assetpool.h"

void Title_Init(Scene* s) {
    
    //Texture* armaan = TexturePool_Get("./assets/textures/armaan.png");
    //Texture_Save(armaan, "test.png");

    // Armaan
    Sprite* spriteArmaan = SpritePool_Get("./assets/textures/armaan.png");
    Transform* transformArmaan = TransformPool_Add((vec2){ 0, 0 }, (vec2){ 1, 1 }, 0);
    
    Component* spriteRendererArmaan = SpriteRenderer_Init(spriteArmaan, (vec4){ 1, 1, 1, 1 }, transformArmaan, -1);
    GameObject* gameObjectArmaan = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectArmaan, spriteRendererArmaan);
    Scene_AddGameObject(s, gameObjectArmaan);

    // Ainsley
    Sprite* spriteAinsley = SpritePool_Get("./assets/textures/ainsley.jpg");
    Transform* transformAinsley = TransformPool_Add((vec2){ 0.5f, 0.5f }, (vec2){ 1, 1 }, 45);

    Component* spriteRendererAinsley = SpriteRenderer_Init(spriteAinsley, (vec4){ 1, 1, 1, 1 }, transformAinsley, 1);
    GameObject* gameObjectAinsley = GameObject_Init(NULL);
    
    GameObject_AddComponent(gameObjectAinsley, spriteRendererAinsley);
    Scene_AddGameObject(s, gameObjectAinsley);

    // Sike
    Sprite* spriteSike = SpritePool_Get("./assets/textures/sike.png");
    Transform* transformSike = TransformPool_Add((vec2){ 0.5f, -0.5f }, (vec2){ 1, 1 }, -45);

    Component* spriteRendererSike = SpriteRenderer_Init(spriteSike, (vec4){ 1, 1, 1, 1 }, transformSike, 0);
    GameObject* gameObjectSike = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectSike, spriteRendererSike);
    Scene_AddGameObject(s, gameObjectSike);

}
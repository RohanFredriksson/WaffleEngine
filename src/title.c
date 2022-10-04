#include "title.h"
#include "assetpool.h"

void Title_Init(Scene* s) {
    
    //Texture* armaan = TexturePool_Get("./assets/textures/armaan.png");
    //Texture_Save(armaan, "test.png");

    // Armaan
    Sprite* spriteArmaan = SpritePool_Get("./assets/textures/armaan.png");
    Transform* transformArmaan = TransformPool_Add((vec2){ 0, 0 }, (vec2){ 1, 1 }, 0);
    
    Component* spriteRendererArmaan = SpriteRenderer_Init(spriteArmaan, (vec4){ 1, 1, 1, 1 }, transformArmaan, 0);
    GameObject* gameObjectArmaan = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectArmaan, spriteRendererArmaan);
    Scene_AddGameObject(s, gameObjectArmaan);

    // Ainsley
    Sprite* spriteAinsley = SpritePool_Get("./assets/textures/ainsley.jpg");
    Transform* transformAinsley = TransformPool_Add((vec2){ 1, 0 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererAinsley = SpriteRenderer_Init(spriteAinsley, (vec4){ 1, 1, 1, 1 }, transformAinsley, 0);
    GameObject* gameObjectAinsley = GameObject_Init(NULL);
    
    GameObject_AddComponent(gameObjectAinsley, spriteRendererAinsley);
    Scene_AddGameObject(s, gameObjectAinsley);

    // Sike
    Sprite* spriteSike = SpritePool_Get("./assets/textures/sike.png");
    Transform* transformSike = TransformPool_Add((vec2){ -1, 0 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererSike = SpriteRenderer_Init(spriteSike, (vec4){ 1, 1, 1, 1 }, transformSike, 0);
    GameObject* gameObjectSike = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectSike, spriteRendererSike);
    Scene_AddGameObject(s, gameObjectSike);

    // Cena
    Sprite* spriteCena = SpritePool_Get("./assets/textures/cena.jpg");
    Transform* transformCena = TransformPool_Add((vec2){ 0, -1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererCena = SpriteRenderer_Init(spriteCena, (vec4){ 1, 1, 1, 1 }, transformCena, 0);
    GameObject* gameObjectCena = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectCena, spriteRendererCena);
    Scene_AddGameObject(s, gameObjectCena);

    // Dre
    Sprite* spriteDre = SpritePool_Get("./assets/textures/dre.jpg");
    Transform* transformDre = TransformPool_Add((vec2){ 1, -1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererDre = SpriteRenderer_Init(spriteDre, (vec4){ 1, 1, 1, 1 }, transformDre, 0);
    GameObject* gameObjectDre = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectDre, spriteRendererDre);
    Scene_AddGameObject(s, gameObjectDre);

    // Notbad
    Sprite* spriteNotbad = SpritePool_Get("./assets/textures/notbad.png");
    Transform* transformNotbad = TransformPool_Add((vec2){ -1, -1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererNotbad = SpriteRenderer_Init(spriteNotbad, (vec4){ 1, 1, 1, 1 }, transformNotbad, 0);
    GameObject* gameObjectNotbad = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectNotbad, spriteRendererNotbad);
    Scene_AddGameObject(s, gameObjectNotbad);

    // Paper
    Sprite* spritePaper = SpritePool_Get("./assets/textures/paper.png");
    Transform* transformPaper = TransformPool_Add((vec2){ 0, 1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererPaper = SpriteRenderer_Init(spritePaper, (vec4){ 1, 1, 1, 1 }, transformPaper, 0);
    GameObject* gameObjectPaper = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectPaper, spriteRendererPaper);
    Scene_AddGameObject(s, gameObjectPaper);
    
    
    // Snake
    Sprite* spriteSnake = SpritePool_Get("./assets/textures/snake.png");
    Transform* transformSnake = TransformPool_Add((vec2){ 1, 1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererSnake = SpriteRenderer_Init(spriteSnake, (vec4){ 1, 1, 1, 1 }, transformSnake, 0);
    GameObject* gameObjectSnake = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectSnake, spriteRendererSnake);
    Scene_AddGameObject(s, gameObjectSnake);
    
    /*
    // Tommy
    Sprite* spriteTommy = SpritePool_Get("./assets/textures/tommy.png");
    Transform* transformTommy = TransformPool_Add((vec2){ -1, 1 }, (vec2){ 1, 1 }, 0);

    Component* spriteRendererTommy = SpriteRenderer_Init(spriteTommy, (vec4){ 1, 1, 1, 1 }, transformTommy, 0);
    GameObject* gameObjectTommy = GameObject_Init(NULL);

    GameObject_AddComponent(gameObjectTommy, spriteRendererTommy);
    Scene_AddGameObject(s, gameObjectTommy);
    */

}
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "assetpool.h"

#define INITIAL_GAMEOBJECTS_SIZE 16

Sprite spriteArmaan;
Sprite spriteAinsley;
Sprite spriteSike;

void Scene_Init(Scene* s, void (*init)(Scene* scene)) {

    s->isRunning = 0;
    s->numGameObjects = 0;
    s->sizeGameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);

    // Temporary

    // Armaan
    Sprite_Init(&spriteArmaan, TexturePool_Get("./assets/textures/armaan.png"));
    Transform* transformArmaan = TransformPool_Add((vec2){ 0, 0 }, (vec2){ 1, 1 }, 0);
    
    Component spriteRendererArmaan;
    SpriteRenderer_Init(&spriteRendererArmaan, &spriteArmaan, (vec4){ 1, 1, 1, 1 }, transformArmaan, -1);
    
    GameObject gameObjectArmaan;
    GameObject_Init(&gameObjectArmaan, NULL);
    GameObject_AddComponent(&gameObjectArmaan, &spriteRendererArmaan);
    Scene_AddGameObject(s, &gameObjectArmaan);

    // Ainsley
    Sprite_Init(&spriteAinsley, TexturePool_Get("./assets/textures/ainsley.jpg"));
    Transform* transformAinsley = TransformPool_Add((vec2){ 0.5f, 0.5f }, (vec2){ 1, 1 }, 45);

    Component spriteRendererAinsley;
    SpriteRenderer_Init(&spriteRendererAinsley, &spriteAinsley, (vec4){ 1, 1, 1, 1 }, transformAinsley, 1);
    
    GameObject gameObjectAinsley;
    GameObject_Init(&gameObjectAinsley, NULL);
    GameObject_AddComponent(&gameObjectAinsley, &spriteRendererAinsley);
    Scene_AddGameObject(s, &gameObjectAinsley);

    // Sike
    Sprite_Init(&spriteSike, TexturePool_Get("./assets/textures/sike.png"));
    Transform* transformSike = TransformPool_Add((vec2){ 0.5f, -0.5f }, (vec2){ 1, 1 }, -45);

    Component spriteRendererSike;
    SpriteRenderer_Init(&spriteRendererSike, &spriteSike, (vec4){ 1, 1, 1, 1 }, transformSike, 0);
    
    GameObject gameObjectSike;
    GameObject_Init(&gameObjectSike, NULL);
    GameObject_AddComponent(&gameObjectSike, &spriteRendererSike);
    Scene_AddGameObject(s, &gameObjectSike);

}

void Scene_Start(Scene* s) {

    // Add all gameobjects to the renderer.
    for (int i = 0; i < s->numGameObjects; i++) {
        Renderer_AddGameObject(&s->renderer, s->gameObjects + i);
    }
    s->isRunning = 1;

}

void Scene_Update(Scene* s, float dt) {

    // Update all gameobjects.
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Update(s->gameObjects + i, dt);
    }

}

void Scene_Render(Scene* s) {
    Renderer_Render(&s->renderer);
}

void Scene_Free(Scene* s) {

    // Free all gameobject data.
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Free(s->gameObjects + i);
    }
    free(s->gameObjects);

    // Free all renderer data.
    Renderer_Free(&s->renderer);

}

void Scene_AddGameObject(Scene* s, GameObject* go) {

    // If there is no more space, allocate some more.
    if (s->numGameObjects == s->sizeGameObjects) {
        s->gameObjects = (GameObject*) realloc(s->gameObjects, s->sizeGameObjects * 2 * sizeof(GameObject));
        s->sizeGameObjects = s->sizeGameObjects * 2;
    }

    // Add the gameobject.
    memcpy(s->gameObjects + s->numGameObjects, go, sizeof(GameObject));
    
    // If the scene is running, add all renderable components to the renderer.
    if (s->isRunning) {
        Renderer_AddGameObject(&s->renderer, s->gameObjects + s->numGameObjects);
    }
    
    s->numGameObjects++;

}
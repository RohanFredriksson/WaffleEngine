#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "texturepool.h"
#include "shaderpool.h"

#define INITIAL_GAMEOBJECTS_SIZE 16

Shader shader;
Sprite testSprite;
Component testSpriteRenderer;
Transform testTransform;
GameObject testGameObject;

void Scene_Init(Scene* s, void (*init)(Scene* scene)) {

    s->numGameObjects = 0;
    s->sizeGameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);

    // Temporary
    Sprite_Init(&testSprite, "./assets/textures/armaan.png");

    Transform_Init(&testTransform, (vec2){ 0, 0 }, (vec2){ 1, 1 }, 0);
    SpriteRenderer_Init(&testSpriteRenderer, &testSprite, (vec4){ 1, 1, 1, 1 }, &testTransform, 0);
    
    GameObject_Init(&testGameObject, NULL);
    GameObject_AddComponent(&testGameObject, &testSpriteRenderer);
    Scene_AddGameObject(s, &testGameObject);

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
    
    // Add all renderable components to the renderer.
    Renderer_AddGameObject(&s->renderer, s->gameObjects + s->numGameObjects);
    
    s->numGameObjects++;

}
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "assetpool.h"

#define INITIAL_GAMEOBJECTS_SIZE 16

void Scene_Init(Scene* s, void (*init)(Scene* scene)) {

    s->isRunning = 0;
    s->numGameObjects = 0;
    s->sizeGameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = (GameObject**) malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject*));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);
    PhysicsSystem2D_Init(&s->physics, 1.0f / 60.0f, (vec2) { 0.0f, 0.0f });

    if (init != NULL) {
        init(s);
    }

}

void Scene_Start(Scene* s) {

    // Add all gameobjects to the renderer.
    for (int i = 0; i < s->numGameObjects; i++) {
        Renderer_AddGameObject(&s->renderer, s->gameObjects[i]);
        PhysicsSystem2D_AddGameObject(&s->physics, s->gameObjects[i]);
    }
    s->isRunning = 1;

}

void Scene_Update(Scene* s, float dt) {

    Camera_AdjustProjection(&s->camera);

    // Update all gameobjects.
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Update(s->gameObjects[i], dt);
    }

    PhysicsSystem2D_Update(&s->physics, dt);

}

void Scene_Render(Scene* s) {
    Renderer_Render(&s->renderer);
    PhysicsSystem2D_Render(&s->physics);
}

void Scene_Free(Scene* s) {

    // Free all gameobject data.
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Free(s->gameObjects[i]);
        free(s->gameObjects[i]);
    }
    free(s->gameObjects);

    // Free all renderer data.
    Renderer_Free(&s->renderer);

}

void Scene_AddGameObject(Scene* s, GameObject* go) {

    // If there is no more space, allocate some more.
    if (s->numGameObjects == s->sizeGameObjects) {
        s->gameObjects = (GameObject**) realloc(s->gameObjects, s->sizeGameObjects * 2 * sizeof(GameObject*));
        s->sizeGameObjects = s->sizeGameObjects * 2;
    }

    // Add the gameobject.
    s->gameObjects[s->numGameObjects] = go;
    
    // If the scene is running, add all renderable components to the renderer.
    if (s->isRunning) {
        Renderer_AddGameObject(&s->renderer, go);
        PhysicsSystem2D_AddGameObject(&s->physics, go);
    }
    
    s->numGameObjects++;

}

GameObject* Scene_GetGameObjectByID(Scene* s, int id) {

    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject* currentGameObject = s->gameObjects[i];
        if (currentGameObject->id == id) {
            return currentGameObject;
        }
    }

    return NULL;
}
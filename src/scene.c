#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"

#define INITIAL_GAMEOBJECTS_SIZE 16

void Scene_Init(Scene* s) {
    s->numGameObjects = 0;
    s->sizeGameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject));
    Camera_Init(&s->camera);
}

void Scene_Update(Scene* s, float dt) {
    
    // Update all gameobjects.
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Update(s->gameObjects + i, dt);
    }

}

void Scene_Render(Scene* s) {
    //Renderer_Render(s->renderer);
}

void Scene_Free(Scene* s) {

    // Free all gameobject data in the array
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Free(s->gameObjects + i);
    }

    // Free all renderer data.
    //Renderer_Free(s->renderer);

    // Free the array itself
    free(s->gameObjects);

}

void Scene_AddGameObject(Scene* s, GameObject* go) {

    // If there is no more space, allocate some more.
    if (s->numGameObjects == s->sizeGameObjects) {
        s->gameObjects = (GameObject*) realloc(s->gameObjects, s->sizeGameObjects * 2 * sizeof(GameObject));
        s->sizeGameObjects = s->sizeGameObjects * 2;
    }

    // Add the gameobject.
    memcpy(s->gameObjects + s->numGameObjects, go, sizeof(GameObject));
    s->numGameObjects++;

}
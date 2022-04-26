#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"

#define INITIAL_GAMEOBJECTS_SIZE 16
int Scene_nextId = 0;

void Scene_Init(Scene* s) {
    s->id = Scene_nextId;
    s->num_gameObjects = 0;
    s->size_gameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject));
    Camera_Init(&s->camera);
    Scene_nextId++;
}

void Scene_Update(Scene* s, float dt) {
    
    // Update all gameobjects.
    for (int i = 0; i < s->num_gameObjects; i++) {
        GameObject_Update(s->gameObjects + i, dt);
    }

}

void Scene_Render(Scene* s) {
    //Renderer_Render(s->renderer);
}

void Scene_Free(Scene* s) {

    // Free all gameobject data in the array
    for (int i = 0; i < s->num_gameObjects; i++) {
        GameObject_Free(s->gameObjects + i);
    }

    // Free all renderer data.
    //Renderer_Free(s->renderer);

    // Free the array itself
    free(s->gameObjects);

}

void Scene_AddGameObject(Scene* s, GameObject* go) {

    // If there is no more space, allocate some more.
    if (s->num_gameObjects == s->size_gameObjects) {
        s->gameObjects = (GameObject*) realloc(s->gameObjects, s->size_gameObjects * 2 * sizeof(GameObject));
        s->size_gameObjects = s->size_gameObjects * 2;
    }

    // Add the gameobject.
    memcpy(s->gameObjects + s->num_gameObjects, go, sizeof(GameObject));
    s->num_gameObjects++;

}
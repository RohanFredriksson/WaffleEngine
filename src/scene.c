#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"

#include "texture.h"
#include "sprite.h"
#include "gameobject.h"
#include "spriterenderer.h"
#include "transform.h"
#include "shader.h"

#define INITIAL_GAMEOBJECTS_SIZE 16

Shader shader;
Texture testTexture;
Sprite testSprite;
Component testSpriteRenderer;
Transform testTransform;
GameObject testGameObject;

void Scene_Init(Scene* s) {

    s->numGameObjects = 0;
    s->sizeGameObjects = INITIAL_GAMEOBJECTS_SIZE;
    s->gameObjects = malloc(INITIAL_GAMEOBJECTS_SIZE * sizeof(GameObject));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);

    // Temporary
    Shader_Init(&shader, "./assets/shaders/default.vert", "./assets/shaders/default.frag");
    Shader_Compile(&shader);
    Renderer_BindShader(&shader);

    Texture_Init(&testTexture, "./assets/textures/armaan.png");
    //Sprite_Init(&testSprite, &testTexture);
    Sprite_Init(&testSprite, NULL);

    vec2 testPosition = { 0, 0 };
    vec2 testSize = { 1, 1 };
    float testRotation = 0;
    Transform_Init(&testTransform, testPosition, testSize, testRotation);

    vec4 testColour = { 1, 0, 1, 1 };
    int testZIndex = 0;
    SpriteRenderer_Init(&testSpriteRenderer, &testSprite, testColour, &testTransform, testZIndex);
    
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

    // Free all gameobject data in the array
    for (int i = 0; i < s->numGameObjects; i++) {
        GameObject_Free(s->gameObjects + i);
    }

    // Free all renderer data.
    Renderer_Free(&s->renderer);

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
    
    // Add all renderable components to the renderer.
    Renderer_AddGameObject(&s->renderer, s->gameObjects + s->numGameObjects);
    
    s->numGameObjects++;

}
#include <stdbool.h>
#include "gameobject.h"
#include "renderer.h"
#include "camera.h"

#ifndef SCENE_H
#define SCENE_H

struct Scene {
    bool isRunning;
    int numGameObjects;
    int sizeGameObjects;
    GameObject* gameObjects;
    Camera camera;
    Renderer renderer;
};
typedef struct Scene Scene;

void Scene_Init(Scene* s, void (*init)(Scene* scene));

void Scene_Start(Scene* s);

void Scene_Update(Scene* s, float dt);

void Scene_Render(Scene* s);

void Scene_Free(Scene* s);

void Scene_AddGameObject(Scene* s, GameObject* go);

#endif
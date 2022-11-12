#include <stdbool.h>
#include "gameobject.h"
#include "renderer.h"
#include "physicssystem.h"
#include "camera.h"

#ifndef SCENE_H
#define SCENE_H

struct Scene {
    bool isRunning;
    int numGameObjects;
    int sizeGameObjects;
    GameObject** gameObjects;
    Camera camera;
    Renderer renderer;
    PhysicsSystem physics;
};
typedef struct Scene Scene;

void Scene_Init(Scene* s, void (*init)(Scene* scene));

void Scene_Start(Scene* s);

void Scene_Update(Scene* s, float dt);

void Scene_Render(Scene* s);

void Scene_Free(Scene* s);

void Scene_AddGameObject(Scene* s, GameObject* go);

GameObject* Scene_GetGameObjectByID(Scene* s, int id);

#endif
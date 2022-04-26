#include "gameobject.h"
#include "camera.h"

#ifndef SCENE_H
#define SCENE_H

struct Scene {
    int id;
    int num_gameObjects;
    int size_gameObjects;
    GameObject* gameObjects;
    Camera camera;
};
typedef struct Scene Scene;

void Scene_Init(Scene* s);

void Scene_Update(Scene* s, float dt);

void Scene_Render(Scene* s);

void Scene_Free(Scene* s);

void Scene_AddGameObject(Scene* s, GameObject* go);

#endif
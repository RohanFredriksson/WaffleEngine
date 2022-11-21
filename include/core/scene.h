#include <stdbool.h>
#include "entity.h"
#include "renderer.h"
#include "physicssystem.h"
#include "camera.h"

#ifndef SCENE_H
#define SCENE_H

struct Scene {
    bool isRunning;
    int numEntities;
    int sizeEntities;
    Entity** entities;
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

void Scene_AddEntity(Scene* s, Entity* entity);

Entity* Scene_GetEntityByID(Scene* s, int id);

#endif
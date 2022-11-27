#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "external.h"
#include "assetpool.h"
#include "entity.h"
#include "renderer.h"
#include "physicssystem.h"
#include "camera.h"
#include "list.h"
#include "wio.h"

#ifndef SCENE_H
#define SCENE_H

struct Scene {
    char* name;
    bool isRunning;
    List entities;
    Camera camera;
    Renderer renderer;
    PhysicsSystem physics;
};
typedef struct Scene Scene;

void Scene_Init(Scene* s, char* name, void (*init)(Scene* scene));

void Scene_Start(Scene* s);

void Scene_Update(Scene* s, float dt);

void Scene_Render(Scene* s);

void Scene_Free(Scene* s);

cJSON* Scene_Serialise(Scene* s);

void Scene_Save(Scene* s);

bool Scene_Load(Scene* s, char* name);

void Scene_AddEntity(Scene* s, Entity* entity);

Entity* Scene_GetEntityByID(Scene* s, int id);

void Scene_RemoveEntityByID(Scene* s, int id);

#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "assetpool.h"

static char path[] = "saves/";
static char extension[] = ".scene";

void Scene_Init(Scene* s, char* name, void (*init)(Scene* scene)) {

    s->name = name;
    s->isRunning = 0;
    List_Init(&s->entities, sizeof(Entity*));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);
    PhysicsSystem_Init(&s->physics, 1.0f / 60.0f, (vec2) { 0.0f, -10.0f });

    if (init != NULL) {
        init(s);
    }

}

void Scene_Start(Scene* s) {

    // Add all entities to the renderer.
    Entity* entity;
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&s->entities, i, &entity);
        Renderer_AddEntity(&s->renderer, entity);
        PhysicsSystem_AddEntity(&s->physics, entity);
    }
    s->isRunning = 1;

}

void Scene_Update(Scene* s, float dt) {

    Camera_AdjustProjection(&s->camera);

    // Update all entities.
    Entity* entity;
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&s->entities, i, &entity);
        Entity_Update(entity, dt);
    }

    PhysicsSystem_Update(&s->physics, dt);

}

void Scene_Render(Scene* s) {
    Renderer_Render(&s->renderer);
    PhysicsSystem_Render(&s->physics);
}

void Scene_Free(Scene* s) {

    // Free all entity data.
    Entity* entity;
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&s->entities, i, &entity);
        Entity_Free(entity);
        free(entity);
    }
    List_Free(&s->entities);

    // Free all renderer data.
    Renderer_Free(&s->renderer);
    PhysicsSystem_Free(&s->physics);

}

cJSON* Scene_Serialise(Scene* s) {

    cJSON* json = cJSON_CreateObject();

    cJSON* sprites = SpritePool_Serialise();
    cJSON_AddItemToObject(json, "sprites", sprites);

    cJSON* entities = cJSON_CreateArray();
    Entity* e;
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&s->entities, i, &e);
        cJSON* entity = Entity_Serialise(e);
        cJSON_AddItemToArray(entities, entity);
    }
    cJSON_AddItemToObject(json, "entities", entities);

    return json;

}

void Scene_Save(Scene* s) {

    char* filename = malloc(strlen(path) + strlen(s->name) + strlen(extension) + 1);
    memcpy(filename, path, strlen(path));
    memcpy(filename + strlen(path), s->name, strlen(s->name));
    memcpy(filename + strlen(path) + strlen(s->name), extension, strlen(extension) + 1);
    printf("%s\n", filename);

    cJSON* json = Scene_Serialise(s);
    char* contents = cJSON_Print(json);
    cJSON_Delete(json);

    printf("%s\n", contents);

    FILE* file = fopen(filename, "w");
    fprintf(file, contents);
    free(contents);
    fclose(file);

}

void Scene_AddEntity(Scene* s, Entity* entity) {

    // Add the entity.
    List_Push(&s->entities, &entity);
    
    // If the scene is running, add all renderable components to the renderer.
    if (s->isRunning) {
        Renderer_AddEntity(&s->renderer, entity);
        PhysicsSystem_AddEntity(&s->physics, entity);
    }

}

Entity* Scene_GetEntityByID(Scene* s, int id) {

    Entity* entity;
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&s->entities, i, &entity);
        if (entity->id == id) {
            return entity;
        }
    }

    return NULL;
}
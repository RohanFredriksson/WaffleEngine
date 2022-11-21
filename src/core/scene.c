#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "assetpool.h"

#define INITIAL_ENTITIES_SIZE 16

void Scene_Init(Scene* s, void (*init)(Scene* scene)) {

    s->isRunning = 0;
    s->numEntities = 0;
    s->sizeEntities = INITIAL_ENTITIES_SIZE;
    s->entities = (Entity**) malloc(INITIAL_ENTITIES_SIZE * sizeof(Entity*));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);
    PhysicsSystem_Init(&s->physics, 1.0f / 60.0f, (vec2) { 0.0f, -10.0f });

    if (init != NULL) {
        init(s);
    }

}

void Scene_Start(Scene* s) {

    // Add all entities to the renderer.
    for (int i = 0; i < s->numEntities; i++) {
        Renderer_AddEntity(&s->renderer, s->entities[i]);
        PhysicsSystem_AddEntity(&s->physics, s->entities[i]);
    }
    s->isRunning = 1;

}

void Scene_Update(Scene* s, float dt) {

    Camera_AdjustProjection(&s->camera);

    // Update all entities.
    for (int i = 0; i < s->numEntities; i++) {
        Entity_Update(s->entities[i], dt);
    }

    PhysicsSystem_Update(&s->physics, dt);

}

void Scene_Render(Scene* s) {
    Renderer_Render(&s->renderer);
    PhysicsSystem_Render(&s->physics);
}

void Scene_Free(Scene* s) {

    // Free all entity data.
    for (int i = 0; i < s->numEntities; i++) {
        Entity_Free(s->entities[i]);
        free(s->entities[i]);
    }
    free(s->entities);

    // Free all renderer data.
    Renderer_Free(&s->renderer);
    PhysicsSystem_Free(&s->physics);

}

void Scene_AddEntity(Scene* s, Entity* entity) {

    // If there is no more space, allocate some more.
    if (s->numEntities == s->sizeEntities) {
        s->entities = (Entity**) realloc(s->entities, s->sizeEntities * 2 * sizeof(Entity*));
        s->sizeEntities = s->sizeEntities * 2;
    }

    // Add the entity.
    s->entities[s->numEntities] = entity;
    
    // If the scene is running, add all renderable components to the renderer.
    if (s->isRunning) {
        Renderer_AddEntity(&s->renderer, entity);
        PhysicsSystem_AddEntity(&s->physics, entity);
    }
    
    s->numEntities++;

}

Entity* Scene_GetEntityByID(Scene* s, int id) {

    for (int i = 0; i < s->numEntities; i++) {
        Entity* currentEntity = s->entities[i];
        if (currentEntity->id == id) {
            return currentEntity;
        }
    }

    return NULL;
}
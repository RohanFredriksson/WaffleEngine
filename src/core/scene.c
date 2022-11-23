#include "scene.h"

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

    cJSON* camera = Camera_Serialise(&s->camera);
    cJSON_AddItemToObject(json, "camera", camera);    

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

bool Scene_Parse(Scene* s, cJSON* json) {
    return 0;
}

void Scene_Save(Scene* s) {

    char* filename = malloc(strlen(path) + strlen(s->name) + strlen(extension) + 1);
    memcpy(filename, path, strlen(path));
    memcpy(filename + strlen(path), s->name, strlen(s->name));
    memcpy(filename + strlen(path) + strlen(s->name), extension, strlen(extension) + 1);

    cJSON* json = Scene_Serialise(s);
    char* contents = cJSON_Print(json);
    cJSON_Delete(json);

    FILE* file = fopen(filename, "w");
    fprintf(file, contents);
    free(filename);
    free(contents);
    fclose(file);

}

bool Scene_Load(Scene* s, char* name) {
    
    // Get the filename.
    char* filename = malloc(strlen(path) + strlen(name) + strlen(extension) + 1);
    memcpy(filename, path, strlen(path));
    memcpy(filename + strlen(path), name, strlen(name));
    memcpy(filename + strlen(path) + strlen(name), extension, strlen(extension) + 1);

    // Get the contents of the save file.
    char* contents = WIO_LoadSource(filename);
    if (contents == NULL) {
        printf("ERROR::SCENE::LOAD::FILE_NOT_FOUND: \"%s\" could not be found.", filename);
        return 0;
    }
    
    // Parse the string into a json object.
    cJSON* json = cJSON_Parse(contents);
    free(contents);
    free(filename);
    if (json == NULL) {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"%s\" could not be parsed.", filename);
        return 0;
    }
    
    // Load the sprites required.
    cJSON* sprites = cJSON_GetObjectItemCaseSensitive(json, "sprites");
    if (sprites == NULL) {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"sprites\" attribute not found.", filename);
        cJSON_Delete(json);
        return 0;
    }
    if (!cJSON_IsArray(sprites)) {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"sprites\" attribute not an array.", filename);
        cJSON_Delete(json);
        return 0;
    }
    cJSON* sprite = NULL;
    cJSON_ArrayForEach(sprite, sprites) {
        Sprite_Load(sprite);
    }

    cJSON_Delete(json);
    return 1;

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
#include "scene.h"

static char path[] = "saves/";
static char extension[] = ".scene";

void Scene_Init(Scene* s, char* name, void (*init)(Scene* scene)) {

    s->name = name;
    List_Init(&s->entities, sizeof(Entity*));
    Camera_Init(&s->camera);
    Renderer_Init(&s->renderer);
    PhysicsSystem_Init(&s->physics, 1.0f / 60.0f, (vec2) { 0.0f, -10.0f });

    if (init != NULL) {
        init(s);
    }

}

static void Scene_AddNewComponents(Scene* s) {

    
    int n = List_Length(&s->entities);
    for (int i = 0; i < n; i++) {
        
        Entity* entity;
        List_Get(&s->entities, i, &entity);
        
        int m = List_Length(&entity->newComponents);
        for (int j = 0; j < m; j++) {
            
            Component* component;
            List_Get(&entity->newComponents, j, &component);

            Renderer_AddComponent(&s->renderer, component);
            PhysicsSystem_AddComponent(&s->physics, component);

        }
        Entity_ClearNewComponents(entity);
    }

}

void Scene_Update(Scene* s, float dt) {

    // Add all new components to the specific systems.
    Scene_AddNewComponents(s);

    // Adjust the projection and step the physics engine.
    Camera_AdjustProjection(&s->camera);
    PhysicsSystem_Update(&s->physics, dt);

    // Update all entities.
    Entity* entity;
    for (int i = 0; i < List_Length(&s->entities); i++) {
        
        List_Get(&s->entities, i, &entity);
        Entity_Update(entity, dt);
        
        // If the entity is dead, remove it.
        if (entity->dead) {
            Entity_Free(entity);
            free(entity);
            List_Remove(&s->entities, i);
            i--;
        }

    }

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

    cJSON* fonts = FontPool_Serialise();
    cJSON_AddItemToObject(json, "fonts", fonts);

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

void Scene_Save(Scene* s) {

    char* filename = malloc(strlen(path) + strlen(s->name) + strlen(extension) + 1);
    memcpy(filename, path, strlen(path));
    memcpy(filename + strlen(path), s->name, strlen(s->name));
    memcpy(filename + strlen(path) + strlen(s->name), extension, strlen(extension) + 1);

    cJSON* json = Scene_Serialise(s);
    char* contents = cJSON_Print(json);
    cJSON_Delete(json);

    //printf("%s\n", contents);

    if (!WIO_WriteFile(filename, contents)) {printf("ERROR::SCENE::SAVE::FILE_WRITE_FAILED\n");}
    free(filename);
    free(contents);

}

bool Scene_Load(Scene* s, char* name) {

    // Initial scene initialisation.
    s->name = name;
    bool success = 0;

    // Get the filename.
    char* filename = malloc(strlen(path) + strlen(name) + strlen(extension) + 1);
    memcpy(filename, path, strlen(path));
    memcpy(filename + strlen(path), name, strlen(name));
    memcpy(filename + strlen(path) + strlen(name), extension, strlen(extension) + 1);

    // Get the contents of the save file.
    char* contents = WIO_LoadSource(filename);
    if (contents == NULL) {
        printf("ERROR::SCENE::LOAD::FILE_NOT_FOUND: \"%s\" could not be found.\n", filename);
        return 0;
    }
    
    // Parse the string into a json object.
    cJSON* json = cJSON_Parse(contents);
    free(contents);
    free(filename);
    if (json == NULL) {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"%s\" could not be parsed.\n", filename);
        return 0;
    }
    
    // Load the required fonts.
    cJSON* fonts = cJSON_GetObjectItemCaseSensitive(json, "fonts");
    if (fonts != NULL && cJSON_IsArray(fonts)) {
        cJSON* font = NULL;
        cJSON_ArrayForEach(font, fonts) {
            Font_Load(font);
        }
    } else {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"fonts\" could not be loaded.\n");
    }

    // Load the required sprites.
    cJSON* sprites = cJSON_GetObjectItemCaseSensitive(json, "sprites");
    if (sprites != NULL && cJSON_IsArray(sprites)) {
        cJSON* sprite = NULL;
        cJSON_ArrayForEach(sprite, sprites) {
            Sprite_Load(sprite);
        }
    } else {
        printf("ERROR::SCENE::LOAD::JSON_PARSE_ERROR: \"sprites\" could not be loaded.\n");
    }
    
    // Load the camera.
    cJSON* camera = cJSON_GetObjectItemCaseSensitive(json, "camera");
    if (camera != NULL) {success = Camera_Load(&s->camera, camera);}
    if (!success) {Camera_Init(&s->camera);}

    // Initialise the renderer and physics engine.
    Renderer_Init(&s->renderer);
    PhysicsSystem_Init(&s->physics, 1.0f / 60.0f, (vec2) { 0.0f, -10.0f });

    // Load the entities.
    List_Init(&s->entities, sizeof(Entity*));
    cJSON* entities = cJSON_GetObjectItemCaseSensitive(json, "entities");
    if (entities != NULL && cJSON_IsArray(entities)) {
        cJSON* entity = NULL;
        cJSON_ArrayForEach(entity, entities) {
            Entity* e = Entity_Parse(entity);
            if (e != NULL) {Scene_AddEntity(s, e);}
        }
    }

    cJSON_Delete(json);
    return 1;

}

void Scene_AddEntity(Scene* s, Entity* entity) {
    List_Push(&s->entities, &entity);
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

void Scene_RemoveEntityByID(Scene* s, int id) {
    Entity* entity = Scene_GetEntityByID(s, id);
    if (entity != NULL) {Entity_Kill(entity);}
}
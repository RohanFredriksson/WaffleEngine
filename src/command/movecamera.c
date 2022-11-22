#include "movecamera.h"

void MoveCamera_Execute(Command* a, Component* c);

Command* MoveCamera_Init(vec2 to, float time) {

    Command* command = Command_Init("MoveCamera", &MoveCamera_Execute, &MoveCamera_Serialise, NULL);
    MoveCamera* m = malloc(sizeof(MoveCamera));

    glm_vec2_copy(to, m->to);
    m->time = time;

    command->data = m;
    return command;

}

void MoveCamera_Execute(Command* a, Component* c) {

    MoveCamera* m = (MoveCamera*) a->data;

    Scene* scene = Window_GetScene();
    Entity* entity;
    int n = List_Length(&scene->entities);
    for (int i = 0; i < n; i++) {
        List_Get(&scene->entities, i, &entity);
        Component* controller = Entity_GetComponent(entity, "CameraController");
        if (controller != NULL) {
            CameraController_MoveTo((CameraController*) controller->data, m->to, m->time);
            return;
        }
    }

}

cJSON* MoveCamera_Serialise(Command* a) {

    MoveCamera* moveCamera = (MoveCamera*) a->data;

    cJSON* json = cJSON_CreateObject();

    cJSON* to = cJSON_CreateArray();
    cJSON* x = cJSON_CreateNumber((double) moveCamera->to[0]);
    cJSON* y = cJSON_CreateNumber((double) moveCamera->to[1]);
    cJSON_AddItemToArray(to, x);
    cJSON_AddItemToArray(to, y);
    cJSON_AddItemToObject(json, "to", to);

    cJSON* time = cJSON_CreateNumber((double) moveCamera->time);
    cJSON_AddItemToObject(json, "time", time);

    return json;

}
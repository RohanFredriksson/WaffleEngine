#include "movecamera.h"

void MoveCamera_Execute(Command* a, Component* c);

Command* MoveCamera_Init(vec2 to, float time) {

    Command* command = Command_Init("MoveCamera");
    MoveCamera* m = malloc(sizeof(MoveCamera));

    glm_vec2_copy(to, m->to);
    m->time = time;

    command->execute = &MoveCamera_Execute;
    command->serialise = &MoveCamera_Serialise;
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
    WIO_AddVec2(json, "to", moveCamera->to);
    WIO_AddFloat(json, "time", moveCamera->time);
    return json;

}
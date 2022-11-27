#include "deletethisentity.h"
#include "window.h"
#include "scene.h"

static void DeleteThisEntity_Execute(Command* a, Component* c) {
    Scene* s = Window_GetScene();
    Scene_RemoveEntityByID(s, c->entity->id);
}

Command* DeleteThisEntity_Init() {
    Command* command = Command_Init("DeleteThisEntity");
    command->execute = &DeleteThisEntity_Execute;
    return command;
}

bool DeleteThisEntity_Load(Command* command, cJSON* json) {
    command->execute = &DeleteThisEntity_Execute;
    return 1;
}
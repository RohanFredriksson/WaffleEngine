#include "deleteentity.h"
#include "window.h"
#include "scene.h"

static void DeleteEntity_Execute(Command* a, Component* c) {
    DeleteEntity* d = (DeleteEntity*) a->data;
    Scene* s = Window_GetScene();
    Scene_RemoveEntityByID(s, d->id);
}

static cJSON* DeleteEntity_Serialise(Command* a) {
    DeleteEntity* d = (DeleteEntity*) a->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "id", d->id);
    return json;
}

DeleteEntity* _DeleteEntity_Init(Command* command, int id) {

    DeleteEntity* d = malloc(sizeof(DeleteEntity));
    d->command = command;
    d->id = id;

    command->execute = &DeleteEntity_Execute;
    command->serialise = &DeleteEntity_Serialise;
    command->data = d;

    return d;

}

Command* DeleteEntity_Init(Entity* e) {
    Command* command = Command_Init("DeleteEntity");
    _DeleteEntity_Init(command, e->id);
    return command;
}

bool DeleteEntity_Load(Command* a, cJSON* json) {
    
    int id;
    if (!WIO_ParseInt(json, "id", &id)) {return 0;}

    // Initialise the delete entity class
    _DeleteEntity_Init(a, id);

    return 1;
}
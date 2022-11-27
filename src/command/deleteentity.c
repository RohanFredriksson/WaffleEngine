#include "deleteentity.h"
#include "window.h"
#include "scene.h"

static void DeleteEntity_Execute(Command* a, Component* c) {
    DeleteEntity* d = (DeleteEntity*) a->data;
    Scene* s = Window_GetScene();
    if (d->deleteType == DELETE_TYPE_THIS) {Entity_Kill(c->entity);}
    else {Scene_RemoveEntityByID(s, d->id);}
}

static cJSON* DeleteEntity_Serialise(Command* a) {
    DeleteEntity* d = (DeleteEntity*) a->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddInt(json, "id", d->id);
    WIO_AddInt(json, "deleteType", d->deleteType);
    return json;
}

DeleteEntity* _DeleteEntity_Init(Command* command, int id, int deleteType) {

    DeleteEntity* d = malloc(sizeof(DeleteEntity));
    d->command = command;
    d->id = id;
    d->deleteType = deleteType;

    command->execute = &DeleteEntity_Execute;
    command->serialise = &DeleteEntity_Serialise;
    command->data = d;

    return d;

}

Command* DeleteEntity_Init(Entity* e) {
    Command* command = Command_Init("DeleteEntity");
    _DeleteEntity_Init(command, e->id, DELETE_TYPE_ID);
    return command;
}

Command* DeleteThisEntity_Init() {
    Command* command = Command_Init("DeleteEntity");
    _DeleteEntity_Init(command, -1, DELETE_TYPE_THIS);
    return command;
}

bool DeleteEntity_Load(Command* a, cJSON* json) {
    
    int id;
    int deleteType;

    if (!WIO_ParseInt(json, "id", &id)) {return 0;}
    if (!WIO_ParseInt(json, "deleteType", &deleteType)) {return 0;}

    // Initialise the delete entity class
    _DeleteEntity_Init(a, id, deleteType);

    return 1;
}
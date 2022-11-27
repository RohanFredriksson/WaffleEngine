#include "command.h"
#include "commands.h"

Command* Command_Init(char* type) {

    Command* a = (Command*) malloc(sizeof(Command));
    a->type = type;
    a->execute = NULL;
    a->serialise = NULL;
    a->free = NULL;
    return a;

}

void Command_Execute(Command* a, Component* c) {
    if (a->execute != NULL) {a->execute(a, c);}
}

cJSON* Command_Serialise(Command* a) {
    
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "type", a->type);
    cJSON* child;
    if (a->serialise != NULL) {child = a->serialise(a);}
    else {child = cJSON_CreateNull();}
    cJSON_AddItemToObject(json, "child", child);
    return json;

}

Command* Command_Load(cJSON* json) {

    char* type;
    if (!WIO_ParseString(json, "type", &type)) {return NULL;}

    // Initialise the command class.
    Command* a = Command_Init(type);

    // Attempt to parse the child class. If cannot, free the command entirely.
    cJSON* child = cJSON_GetObjectItemCaseSensitive(json, "child");
    if (child == NULL) {free(a); return NULL;}
    if (strcmp(type, "MoveCamera") == 0) {if (!MoveCamera_Load(a, child)) {free(a); return NULL;}}
    else if (strcmp(type, "Console") == 0) {if (!Console_Load(a, child)) {free(a); return NULL;}}
    else if (strcmp(type, "DeleteEntity") == 0) {if (!DeleteEntity_Load(a, child)) {free(a); return NULL;}}
    else if (strcmp(type, "DeleteThisEntity") == 0) {if (!DeleteThisEntity_Load(a, child)) {free(a); return NULL;}}
    else {free(a); return NULL;}

    return a;
}

void Command_Free(Command* a) {
    if (a->free != NULL) {a->free(a);}
    if (a->data != NULL) {free(a->data);}
}
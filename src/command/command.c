#include "command.h"

Command* Command_Init(char* type) {

    Command* a = (Command*) malloc(sizeof(Command));
    a->type = StringPool_Get(type);
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

void Command_Free(Command* a) {
    if (a->free != NULL) {a->free(a);}
    if (a->data != NULL) {free(a->data);}
}
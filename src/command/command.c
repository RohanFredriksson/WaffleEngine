#include "command.h"

Command* Command_Init(char* type, 
                      void (*execute) (struct Command* a, Component* c), 
                      cJSON* (*serialise) (struct Command* a),
                      void (*free)(struct Command* a)) {

    Command* a = (Command*) malloc(sizeof(Command));
    a->type = StringPool_Get(type);
    a->execute = execute;
    a->serialise = serialise;
    a->free = free;
    return a;

}

void Command_Execute(Command* a, Component* c) {
    if (a->execute != NULL) {a->execute(a, c);}
}

cJSON* Command_Serialise(Command* a) {
    
    cJSON* json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "type", a->type);
    
    cJSON* child;
    if (a->serialise != NULL) {child = a->serialise(a);}
    else {child = cJSON_CreateObject();}
    cJSON_AddItemToObject(json, "child", child);

    return json;

}

void Command_Free(Command* a) {
    if (a->free != NULL) {a->free(a);}
    if (a->data != NULL) {free(a->data);}
    free(a->type);
}
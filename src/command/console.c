#include "console.h"

Command* Console_Init(char* message) {

    Command* command = Command_Init("Console", &Console_Execute, &Console_Serialise, &Console_Free);
    Console* console = malloc(sizeof(Console));
    
    console->message = malloc(strlen(message) + 1);
    memcpy(console->message, message, strlen(message) + 1);

    command->data = console;
    return command;

}

void Console_Execute(Command* a, Component* c) {
    Console* l = (Console*) a->data;
    printf("%s", l->message);
}

cJSON* Console_Serialise(Command* a) {

    Console* console = (Console*) a->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "message", console->message);
    return json;

}

void Console_Free(Command* a) {
    Console* l = (Console*) a->data;
    free(l->message);
}

#include "console.h"

Command* Console_Init(char* message) {

    Command* command = Command_Init("Console");

    Console* console = malloc(sizeof(Console));
    console->message = StringPool_Get(message);

    command->execute = &Console_Execute;
    command->serialise = &Console_Serialise;
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

bool Console_Load(Command* a, cJSON* json) {

    char* message;
    if (!WIO_ParseString(json, "message", &message)) {return 0;}

    Console* console = malloc(sizeof(Console));
    console->message = StringPool_Get(message);

    a->execute = &Console_Execute;
    a->serialise = &Console_Serialise;
    a->data = console;
    return 1;

}
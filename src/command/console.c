#include "console.h"

static void Console_Execute(Command* a, Component* c) {
    Console* l = (Console*) a->data;
    printf("%s", l->message);
}

static cJSON* Console_Serialise(Command* a) {
    Console* console = (Console*) a->data;
    cJSON* json = cJSON_CreateObject();
    WIO_AddString(json, "message", console->message);
    return json;
}

Console* _Console_Init(Command* command, char* message) {

    Console* console = malloc(sizeof(Console));
    console->message = StringPool_Get(message);

    command->execute = &Console_Execute;
    command->serialise = &Console_Serialise;
    command->data = console;

    return console;
}

Command* Console_Init(char* message) {
    Command* command = Command_Init("Console");
    _Console_Init(command, message);
    return command;
}

bool Console_Load(Command* a, cJSON* json) {

    char* message;
    if (!WIO_ParseString(json, "message", &message)) {return 0;}

    // Initialise the console class.
    _Console_Init(a, message);

    return 1;
}
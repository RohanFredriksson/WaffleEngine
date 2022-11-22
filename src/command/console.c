#include "console.h"

Command* Console_Init(char* message) {

    Command* command = Command_Init("Console", &Console_Execute, &Console_Free);
    Console* console = malloc(sizeof(Console));
    console->message = StringPool_Get(message);
    command->data = console;
    return command;

}

void Console_Execute(Command* a, Component* c) {
    Console* l = (Console*) a->data;
    printf("%s", l->message);
}

void Console_Free(Command* a) {
    Console* l = (Console*) a->data;
    free(l->message);
}

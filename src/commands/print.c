#include "print.h"

Command* Print_Init(char* message) {

    Command* command = Command_Init("Print", &Print_Execute, &Print_Free);
    Print* print = malloc(sizeof(Print));

    print->message = malloc(strlen(message) + 1);
    memcpy(print->message, message, strlen(message) + 1);

    command->data = print;
    return command;

}

void Print_Execute(Command* a, Component* c) {
    Print* p = (Print*) a->data;
    printf("%s", p->message);
}

void Print_Free(Command* a) {
    Print* p = (Print*) a->data;
    free(p->message);
}

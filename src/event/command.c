#include "command.h"

Command* Command_Init(char* type, 
                    void (*execute) (Component* c), 
                    void (*free)(struct Command* a)) {

    Command* a = (Command*) malloc(sizeof(Command));

    // Store the type
    a->type = malloc(strlen(type)+1);
    memcpy(a->type, type, strlen(type)+1);

    a->execute = execute;
    a->free = free;

    return a;

}

void Command_Execute(Command* a, Component* c) {
    if (a->execute != NULL) {a->execute(c);}
}

void Command_Free(Command* a) {
    if (a->free != NULL) {a->free(a);}
    if (a->data != NULL) {free(a->data);}
    free(a->type);
}
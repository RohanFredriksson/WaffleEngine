#include "command.h"

Command* Command_Init(char* type, 
                      void (*execute) (struct Command* a, Component* c), 
                      void (*free)(struct Command* a)) {

    Command* a = (Command*) malloc(sizeof(Command));
    a->type = StringPool_Get(type);
    a->execute = execute;
    a->free = free;
    return a;

}

void Command_Execute(Command* a, Component* c) {
    if (a->execute != NULL) {a->execute(a, c);}
}

void Command_Free(Command* a) {
    if (a->free != NULL) {a->free(a);}
    if (a->data != NULL) {free(a->data);}
    free(a->type);
}
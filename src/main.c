#include "window.h"
#include <stdlib.h>

#include <stdio.h>
#include <pthread.h>

void* test(void* args) {
    printf("HELLO\n");
    return NULL;
}

int main() {

    pthread_t id;
    pthread_create(&id, NULL, test, NULL);

    Window_Run();
}
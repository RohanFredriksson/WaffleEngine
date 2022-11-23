#include "wio.h"

char* WIO_LoadSource(char* file) {
    
    FILE* f = fopen(file, "r");
    if (f == NULL) {return NULL;}

    size_t size = 16;
    size_t length = 0;
    char* source = malloc(size);

    char next;
    while (1) {
        
        next = fgetc(f);
        if (next == EOF) {
            break;
        }

        if (length >= size) {
            source = realloc(source, size * 2);
            size = size * 2;
        }

        source[length] = next;
        length++;

    }

    fclose(f);
    source = realloc(source, length + 1);
    source[length] = '\0';

    return source;   
}

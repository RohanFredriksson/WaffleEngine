#include <stdlib.h>
#include <string.h>
#include "list.h"

#define INITIAL_LIST_SIZE 16

void List_Init(List* l, size_t elementSize) {
    l->elements = malloc(INITIAL_LIST_SIZE * sizeof(void*));
    l->elementSize = elementSize;
    l->size = INITIAL_LIST_SIZE;
    l->length = 0;
}

void List_Free(List* l) {
    for (int i = 0; i < l->length; i++) free(l->elements[i]);
    free(l->elements);
}

int List_Length(List* l) {
    return l->length;
}

int List_Get(List* l, int index, void* buffer) {
    if (index < 0 || index >= l->length) return 1;
    memcpy(buffer, l->elements[index], l->elementSize);
    return 0;
}

int List_Pop(List* l, void* buffer) {
    
    if (l->length < 1) return 1;
    if (buffer != NULL) memcpy(buffer, l->elements[l->length-1], l->elementSize);
    
    free(l->elements[l->length-1]);
    return 0;

}

int List_Shift(List* l, void* buffer) {
    
    if (l->length < 1) return 1;
    if (buffer != NULL) memcpy(buffer, l->elements[0], l->elementSize);
    
    free(l->elements[0]);
    memmove(l->elements, l->elements + 1, (l->length - 1) * sizeof(void*));
    return 0;

}

int List_Remove(List* l, int index) {
    
    if (index < 0 || index >= l->length) return 1;
    if (index == 0) return List_Shift(l, NULL);
    if (index == l->length - 1) return List_Pop(l, NULL);
    
    free(l->elements[index]);
    memmove(l->elements + index, l->elements + index + 1, (l->length - index - 1) * sizeof(void*));
    return 0;

}

void List_Push(List* l, void* element) {

    if (l->length >= l->size) {
        l->elements = realloc(l->elements, l->size * 2 * sizeof(void*));
        l->size = l->size * 2;
    }

    void* e = malloc(l->elementSize);
    memmove(e, element, l->elementSize);
    l->elements[l->length] = e;
    l->length++;

}

void List_Unshift(List* l, void* element) {

    if (l->length >= l->size) {
        l->elements = realloc(l->elements, l->size * 2 * sizeof(void*));
        l->size = l->size * 2;
    }

    void* e = malloc(l->elementSize);
    memmove(e, element, l->elementSize);
    memmove(l->elements + 1, l->elements, l->length * sizeof(void*));
    l->elements[0] = e;
    l->length++;

}

void List_Add(List* l, int index, void* element) {

    if (index < 0) index = 0;
    if (index > l->length) index = l->length;
    
    if (index == 0) {
        List_Unshift(l, element);
        return;
    }

    if (index == l->length) {
        List_Push(l, element);
        return;
    }

    if (l->length >= l->size) {
        l->elements = realloc(l->elements, l->size * 2 * sizeof(void*));
        l->size = l->size * 2;
    }

    void* e = malloc(l->elementSize);
    memmove(e, element, l->elementSize);
    memmove(l->elements + index + 1, l->elements + index, (l->length - index) * sizeof(void*));
    l->elements[index] = e;
    l->length++;

}

void List_Clear(List* l) {
    size_t elementSize = l->elementSize;
    List_Free(l);
    List_Init(l, elementSize);
}
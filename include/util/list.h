#ifndef LIST_H
#define LIST_H

struct List {
    void** elements;
    size_t elementSize;
    int size;
    int length;
};
typedef struct List List;

void List_Init(List* l, size_t elementSize);
void List_Free(List* l);
int List_Length(List* l);
int List_Get(List* l, int index, void* buffer);
int List_Pop(List* l, void* buffer);
int List_Shift(List* l, void* buffer);
int List_Remove(List* l, int index);
void List_Push(List* l, void* element);
void List_Unshift(List* l, void* element);
void List_Add(List* l, int index, void* element);
void List_Clear(List* l);

#endif
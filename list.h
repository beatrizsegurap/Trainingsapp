#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef _list_h
#define _list_h

typedef struct List List;
typedef List Stack;
typedef List Cola;

/* list operations */

List* createList();

int is_empty(List* list);

void* front(List* list);

void* first(List* list);

void* next(List* list);

void* last(List* list);

void* prev(List* list);

void popFront(List* list);

void popBack(List* list);

void pushFront(List* list, void* data);

void pushBack(List* list, void* data);

void pushCurrent(List* list, void* data);

void popCurrent(List* list);

int get_size();


/* stack operations */
Stack* createStack();

void popStack(Stack* s);

void* topStack(Stack* s);

void pushStack(Stack* s, void* data);

/* cola operations */
Cola* createCola();

void popCola(Cola* c);

void pushCola(Cola* c, void* data);

void clean(List * list);

bool vacio(List * list);


#endif /* _list_h */

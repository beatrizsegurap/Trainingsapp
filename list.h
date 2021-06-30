#ifndef List_h
#define List_h

typedef struct Node Node;
typedef struct List List;

List * createList(void);

List * createStack();

List * createCola();

void * firstList(List * list);

void * nextList(List * list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list, void * data);

void pushBack(List * list, void * data);

void pushCurrent(List * list, void * data);

void * popFront(List * list);

void * popBack(List * list);

void * popCurrent(List * list);

void cleanList(List * list);

void pop_stack(List* s);

void* top_stack(List* s);

void push_stack(List* s, void* data);

void pop_cola(List* c);

void push_cola(List* c,void* data);

bool vacio(List * list);

int size(List * list);

#endif /* List_h */
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "list.h"


// structures
typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};


typedef struct List List;

//Creamos una lista 

struct List{
  Node * head;
  Node * current;
  Node * tail;
};


/*
  private function declarations
*/

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
     List * new = (List *)malloc(sizeof(List));
     assert(new != NULL);
     new->head = new->tail = new->current = NULL;
     return new;
}

List * createStack(){
    return createList();
}

List * createCola(){
    return createList();
}

void * firstList(List * list) {
    if (list == NULL || list->head == NULL) return NULL;
    list->current = list->head;
    return (void *)list->current->data;
}

void * nextList(List * list) {
    if (list == NULL || list->head == NULL || list->current == NULL || list->current->next == NULL) return NULL;
    list->current = list->current->next;
    return (void *)list->current->data;
}

void * lastList(List * list) {
    if (list == NULL || list->head == NULL) return NULL;
    list->current = list->tail;
    return (void *)list->current->data;
}

void * prevList(List * list) {
    if (list == NULL || list->head == NULL || list->current == NULL || list->current->prev == NULL) return NULL;
    list->current = list->current->prev;
    return (void *)list->current->data;
}

void pushFront(List * list,void * data) {
    assert(list != NULL);
    
    Node * new = createNode(data);
    
    if (list->head == NULL) {
        list->tail = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
    }
    
    list->head = new;
}


void pushCurrent(List * list,void * data) {
    assert(list != NULL && list->current !=NULL);
    Node * new = createNode(data);

    if(list->current->next)
        new->next = list->current->next;
    new->prev = list->current;

    if(list->current->next)
        list->current->next->prev = new;
    list->current->next = new;

    if(list->current==list->tail)
        list->tail=new;

}

void pushBack(List * list,void * data) {
    list->current = list->tail;
    if(list->current==NULL) pushFront(list,data);
    else pushCurrent(list,data);
}

void * popCurrent(List * list) {
    assert(list != NULL || list->head != NULL);
    
    if (list->current == NULL) return NULL;
    
    Node * aux = list->current;
    
    if (aux->next != NULL) 
        aux->next->prev = aux->prev;
    
    
    if (aux->prev != NULL) 
        aux->prev->next = aux->next;
    
    
    void * data = (void *)aux->data;
    
    if(list->current == list->tail)
        list->tail = list->current->prev;

    if(list->current == list->head)
        list->head = list->current->next;
        
    list->current = aux->prev;
  
    free(aux);
    
    return data;
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void cleanList(List * list) {
    assert(list != NULL);
    
    while (list->head != NULL) {
        popFront(list);
    }
}

void pop_stack(List* s){
  popBack(s);
}

void* top_stack(List* s){
  return lastList(s);
}

void push_stack(List* s, void* data){
    pushBack(s,data);
}

void pop_cola(List* c){
    popFront(c);
}

void push_cola(List* c, void* data){
    pushBack(c,data);
}

bool vacio(List * list){
  if(!list->head){
    return true;
  }
  return false;
}

int size(List * list){
    int cont=1;
    list->current=list->head;
    while(list->current!=list->tail){
        cont++;
        nextList(list);
    }
    return cont;
}
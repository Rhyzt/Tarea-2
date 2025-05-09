#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

Node *createNode(void * data) {
    Node *new = (Node *)malloc(sizeof(Node));
    new -> data = data;
    new -> prev = NULL;
    new -> next = NULL;
    return new;
}

List *createList() {
    List *L = (List *)malloc(sizeof(List));
    L -> head = NULL;
    L -> tail = NULL;
    L -> current = NULL;
     return L;
}

void *firstList(List * list) {
    list -> current = list -> head;
    if (list -> current == NULL)
        return NULL;
    return list -> current -> data;
}

void *nextList(List * list) {
    if (list -> current == NULL || list -> current -> next == NULL)
        return NULL;
    list -> current = list -> current -> next;
    return list -> current -> data;
}

void *lastList(List * list) {
    if (list -> tail == NULL)
        return NULL;
    list -> current = list -> tail;
    return list -> current -> data;
}

void *prevList(List * list) {
    if (list -> current == NULL || list -> current -> prev == NULL)
        return NULL;
    list -> current = list -> current -> prev;
    return list -> current -> data;
}

void pushFront(List * list, void * data) {
    Node *nuevoNodo = createNode(data);
    if (list -> head == NULL) {
        list -> head = nuevoNodo;
        list -> tail = nuevoNodo;
        list -> current = nuevoNodo;
        return;
    }
    list -> head -> prev = nuevoNodo;
    nuevoNodo -> next = list -> head;
    list -> head = nuevoNodo;
}

void createFirstNode(List* list, Node* nodo) {
    list -> head = nodo;
    list -> tail = nodo;
    list -> current = nodo;
}

void pushBack(List * list, void * data) {
    Node *nuevoNodo = createNode(data);
    if (list -> head == NULL) {
        createFirstNode(list, nuevoNodo);
        return;
    }
    list -> tail -> next = nuevoNodo;
    nuevoNodo -> prev = list -> tail;
    list -> tail = nuevoNodo;
}

void *popFront(List * list) {
    list->current = list->head;
    return(popCurrent(list));
}

void *popCurrent(List * list) {
    if (list -> current == NULL || list -> head == NULL) return NULL;
    Node *aux;
    void *dato = NULL;
    if (list -> current -> prev == NULL) { // Si es el primer elemento
        if (list -> current -> next != NULL) { // Si no es el unico elemento
            list -> current -> next -> prev = NULL;
            list -> head = list -> current -> next;
            aux = list -> current -> next;
        } else { // Es el unico elemento
            list -> head = NULL;
            list -> tail = NULL;
            aux = NULL; // La lista queda vacía
        }
    }
    else if (list -> current -> next == NULL) { // Ultimo elemento
        list -> current -> prev -> next = NULL;
        list -> tail = list -> current -> prev;
        aux = list -> current -> prev;

    }
    else { // En medio de la lista
        list -> current -> prev -> next = list -> current -> next;
        list -> current -> next -> prev = list -> current -> prev;
        aux = list -> current -> next;
    }
    dato = list -> current -> data;
    free(list -> current); // Libera el nodo
    list -> current = aux;
    return dato;
}

void *getCurrentData(List *list) {
    if (list -> current == NULL || list -> head == NULL) return NULL;
    return list -> current -> data;
}
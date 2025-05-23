#ifndef list_h
#define list_h

typedef struct Node Node;
struct Node {
    void *data;
    Node *next;
    Node *prev;
};

typedef struct List List;
struct List {
    Node *head;
    Node *tail;
    Node *current;
};

List * createList(void);

void * firstList(List * list);

void * nextList(List * list);

void * lastList(List * list);

void * prevList(List * list);

void pushFront(List * list, void * data);

void pushBack(List * list, void * data);

void pushCurrent(List * list, void * data);

void *popFront(List * list);

void *popCurrent(List * list);

void *getCurrentData(List *list);

#endif /* List_h */
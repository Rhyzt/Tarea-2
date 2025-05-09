#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "mapa.h"

Pair *createPair(char * key, void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new -> key = strdup(key); 
    new -> value = value;
    return new;
}

unsigned long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(Map * map, char * key, void * value) {    
    int pos = hash(key, map -> capacity);
    if (map -> pares[pos] == NULL || map -> pares[pos] -> key == NULL) {
        map -> pares[pos] = createPair(key, value);
        Pair *checkPair = map -> pares[pos];
        map -> current = pos;
    }
    else {
        while (map -> pares[pos] != NULL) {
            if (strcmp(map -> pares[pos] -> key, key) == 0) return;
            pos = (pos + 1) % (map -> capacity);
        }
        map -> pares[pos] = createPair(key, value);
    }
    (map -> size)++;
    return;
}

void enlarge(Map * map) {
    Pair **arrayAntiguo = map -> pares;
    (map -> capacity) *= 2;
    map -> size = 0;
    map -> current = -1;
    Pair **arrayNuevo = calloc(map -> capacity, sizeof(Pair *));
    map -> pares = arrayNuevo;
    for(int i = 0 ; i < (map -> capacity)/2 ; i++) {
        if(arrayAntiguo[i] != NULL && arrayAntiguo[i] -> key != NULL) {
            insertMap(map, arrayAntiguo[i] -> key, arrayAntiguo[i] -> value);
            free(arrayAntiguo[i]);
            arrayAntiguo[i] = NULL;
        }
    }
    free(arrayAntiguo);
    arrayAntiguo = NULL;
}


Map *createMap(long capacity) {
    Map *mapa = malloc(sizeof(Map));
    mapa -> pares = calloc(capacity, sizeof(Pair *));
    mapa -> size = 0;
    mapa -> capacity = capacity;
    mapa -> current = -1;
    return mapa;
}



void eraseMap(Map * map,  char * key) {    
    int pos = hash(key, map -> capacity);
    while (map -> pares[pos] != NULL) {
        if (map -> pares[pos] -> key == NULL)
            if (strcmp(map -> pares[pos] -> key,key) == 0) {
                map -> pares[pos] -> key = NULL;
                (map -> size)--;
                return;
            }
        pos = (pos + 1) % map -> capacity;
    }   
}

Pair *searchMap(Map *map,  char *key) {  
    int pos = hash(key, map -> capacity);
    while (map -> pares[pos] != NULL) {
        if (map -> pares[pos] -> key != NULL && strcmp(map -> pares[pos] -> key, key) == 0) {
            map -> current = pos;
            Pair *checkPair = map -> pares[pos];
            return map -> pares[pos];
        }
        pos = (pos + 1) % map -> capacity;
    }
    return NULL;
}

Pair *firstMap(Map *map) {
    map -> current = -1;
    for (int i = 0 ; i < map -> capacity ; i++) {
        if (map -> pares[i] != NULL && map -> pares[i] -> key != NULL) {
            map -> current = i;
            return map -> pares[i];
        }
    }
    return NULL;
}

Pair *nextMap(Map *map) {
    if (map -> current == -1) return NULL;
    int pos = (map -> current) + 1;
    while (pos < map -> capacity) { 
        if (map -> pares[pos] != NULL && map -> pares[pos] -> key != NULL) {
            map -> current = pos;
            return map -> pares[pos];
        }
        pos++;
    }
    return NULL;
}

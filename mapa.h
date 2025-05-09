#ifndef mapa_h
#define mapa_h


typedef struct Map Map;


typedef struct Pair {
     char *key;
     void *value;
} Pair;

struct Map {
    Pair **pares;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Map *createMap(long capacity);

void insertMap(Map * table, char * key, void * value);

void eraseMap(Map * table, char * key);

void sizeDown(Map* map);

Pair *searchMap(Map * table, char * key);

Pair *firstMap(Map * table);

Pair *nextMap(Map * table);

void enlarge(Map * map);

#endif /* mapa_h */

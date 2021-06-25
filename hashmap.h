#ifndef HashMap_h
#define HashMap_h


typedef struct Pair Pair;
typedef struct HashMap HashMap;

Pair * createPair( char * key,  void * value);

long hash( char * key, long capacity);

int is_equal(void* key1, void* key2);

HashMap * createMap(long capacity);

void insertMap(HashMap * table, char * key, void * value);

void eraseMap(HashMap * table, char * key);

void * searchMap(HashMap * table, char * key);

void * firstMap(HashMap * map);

void * nextMap(HashMap * table);

void enlarge(HashMap * map);

long sizeMap(HashMap * map);

long capacity(HashMap * map);

#endif
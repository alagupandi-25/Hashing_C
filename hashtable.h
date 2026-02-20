#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct pair Pair;
typedef struct hashtable HashTable;

HashTable* initMap();
void rehash(HashTable*);
char* get(HashTable*,char*);
void put(HashTable*,char*,char*);
int contains(HashTable*,char*);
void deleteKey(HashTable*,char*);
void freeHashTable(HashTable*);

#endif
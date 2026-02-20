#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

/*
Hashing Algorithm : Polynomial Rolling Hash;
Support Ascii Only;
Note that user is responsible for String memory management;
*/

int growthSize[] = {521,1049,2111,4229,8461,16927,33857,67723,135457,270923,541849};
int growthSizeN = 11;

struct pair{
    char* key;
    char* data;
    struct pair* next;
};

struct hashtable{
    double growthFactor;
    Pair** buckets;
    int size;
    int gindex;
};

#include "hashtable.h"

unsigned long long hashFunction(char* s){
    uint64_t p = 263ull;
    uint64_t m = 9999991ull;
    uint64_t hash = 0ull;
    uint64_t powP = 1ull;
    uint32_t i = 0u;
    while(s[i] != 0){
        hash = (hash + ((unsigned char)s[i++] + 1) * powP) % m;
        powP = (powP * p) % m;
    }
    return hash;
}

HashTable* initMap(){
    HashTable* t = malloc(sizeof(HashTable));
    t->growthFactor = 0.8;
    t->gindex = t->size = 0;
    t->buckets = calloc(growthSize[t->gindex],sizeof(Pair*));;
    return t;
}

Pair* createPair(char* key,char* data){
    Pair* t = malloc(sizeof(Pair));
    t->key = key;
    t->data = data;
    return t;
} 

void putRehash(Pair** buckets,Pair* data,int growthSize){
    unsigned long long hash = hashFunction(data->key);
    int idx = hash % growthSize;
    if(buckets[idx] == NULL){
        buckets[idx] = data;
        return;
    }
    Pair* temp = buckets[idx];
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = data;
}

void rehash(HashTable* map){
    if(map->gindex >= growthSizeN - 1)
        return;
    int oldSize = growthSize[map->gindex];
    map->gindex++;
    Pair** newBuckets = calloc(growthSize[map->gindex],sizeof(Pair*));
    for(int i = 0 ; i < oldSize ; ++i){
        Pair* temp = map->buckets[i];
        while (temp != NULL){
            Pair* data = temp;
            temp = temp->next;
            data->next = NULL;
            putRehash(newBuckets,data,growthSize[map->gindex]);
        }
    } 
    free(map->buckets);
    map->buckets = newBuckets;
}

char* get(HashTable* map,char* key){
    unsigned long long hash = hashFunction(key);
    int idx = hash % growthSize[map->gindex];
    Pair* temp = map->buckets[idx];
    while (temp != NULL){
        if(strcmp(temp->key,key) == 0)
            return temp->data;
        temp = temp->next;
    }
    return NULL;
}

void put(HashTable* map,char* key,char* data){
    if(map->gindex < growthSizeN - 1 && 
        map->size >= growthSize[map->gindex] * map->growthFactor)
        rehash(map);
    unsigned long long hash = hashFunction(key);
    int idx = hash % growthSize[map->gindex];
    if(map->buckets[idx] == NULL){
        map->buckets[idx] = createPair(key, data);
        map->size++;
        return;
    }
    Pair* prev = NULL;
    Pair* temp = map->buckets[idx];
    while (temp != NULL){
        if(strcmp(temp->key,key) == 0){
            temp->data = data;
            return;
        }
        temp = temp->next;
    }
    prev->next = createPair(key, data);
    map->size++;
}

int contains(HashTable* map,char* key){
    unsigned long long hash = hashFunction(key);
    int idx = hash % growthSize[map->gindex];
    Pair* temp = map->buckets[idx];
    while (temp != NULL){
        if(strcmp(temp->key,key) == 0)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void deleteKey(HashTable* map,char* key){
    unsigned long long hash = hashFunction(key);
    int idx = hash % growthSize[map->gindex];
    Pair* temp = map->buckets[idx];
    if(temp == NULL)
        return;
    if(strcmp(temp->key,key) == 0){
        map->buckets[idx] = temp->next;
        free(temp);
        map->size--;
        return;
    }
    Pair* prev = temp;
    temp = temp->next;
    while (temp != NULL){
        if(strcmp(temp->key,key) == 0){
            prev->next = temp->next;
            free(temp);
            map->size--;
        }
        prev = temp;
        temp = temp->next;
    }
}

void freeHashTable(HashTable* map) {
    for (int i = 0; i < growthSize[map->gindex]; ++i) {
        Pair* temp = map->buckets[i];
        while (temp != NULL) {
            Pair* data = temp;
            temp = temp->next;
            free(data);  
        }
    }
    free(map->buckets); 
    free(map); 
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned long long hashFunction(char* s){
    uint64_t p = 263ull;
    uint64_t m = 1000000009ull;
    uint64_t hash = 0ull;
    uint64_t powP = 1ull;
    uint32_t i = 0u;
    while(s[i] != 0){
        hash = (hash + ((unsigned char)s[i++] + 1) * powP) % m;
        powP = (powP * p) % m;
    }
    return hash;
}

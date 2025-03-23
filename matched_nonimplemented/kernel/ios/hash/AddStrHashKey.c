#include "iosHash.h"


void AddStrHashKey(volatile t_hashTable* hTable, volatile t_hashedSym* hSym)
{
    u32 maxVal;
    u32 hash;
    u32 off;

    maxVal = hTable->maxValue - 1;
    hash = hSym->hash;
    off = hash & maxVal;
    
    hSym->next = hTable->table[off];
    hTable->table[hash & (hTable->maxValue - 1)] = (t_hashedSym*)hSym;
}


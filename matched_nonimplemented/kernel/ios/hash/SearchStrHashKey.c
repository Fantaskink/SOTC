#include "iosHash.h"


t_hashedSym* SearchStrHashKey(t_hashTable* hTable, u32 hash)
{
    
    t_hashedSym* cSym = hTable->table[hash & (hTable->maxValue - 1)];
    
    while (cSym != NULL)
    {
        if (hash == cSym->hash)
            return cSym;
        
        cSym = cSym->next;
    }
    
    return NULL;
}


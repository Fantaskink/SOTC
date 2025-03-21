#include "iosHash.h"


void iosFree(void*);

void ReleaseStrHashKeyAll(t_hashTable* hTable)
{
    iosFree(hTable->table);
    hTable->table = NULL;
    hTable->maxValue = 0;
}


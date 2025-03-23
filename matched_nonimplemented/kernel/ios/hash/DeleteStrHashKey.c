#include "iosHash.h"


extern void iosJumpRecoverPoint(const char* msg, ...);
extern char D_40048F98;


void DeleteStrHashKey(t_hashTable* hTable, t_hashedSym* sym)
{
    t_hashedSym* nSym;
    t_hashedSym* pSym;
    t_hashedSym* cSym;
    
    cSym = hTable->table[sym->hash & (hTable->maxValue - 1)];
    pSym = NULL;
    
    while (cSym != NULL)
    {
        nSym = cSym->next;
        if (sym == cSym)
        {
            if (pSym == NULL)
                hTable->table[cSym->hash & (hTable->maxValue - 1)] = nSym;
            
            else
                pSym->next = nSym;
            
            return;
        }
        
        pSym = cSym;
        cSym = nSym;
    }
    
    iosJumpRecoverPoint(&D_40048F98);
}


#ifndef IOS_HASH_H
#define IOS_HASH_H

#include "common.h"
#include "fl_xfftype.h"



typedef struct t_hashedSym {
    u32 hash;
    struct t_xffSymEnt* extSym;
    struct t_xffSymEnt* locSym;
    struct t_hashedSym* next;
} t_hashedSym;

typedef struct {
    u32 maxValue;
    t_hashedSym** table;
} t_hashTable;

#endif // IOS_HASH_H

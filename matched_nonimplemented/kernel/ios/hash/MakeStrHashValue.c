#include "iosHash.h"

extern int strcmp(const char* a, const char* b);
extern int strlen(const char* str);

extern const char D_40048FC8;

u32 MakeStrHashValue(const char* str)
{
    int i;
    u32 j;
    u32 hash;

    hash = 0;
    
    if (str == NULL || strcmp(&D_40048FC8, str) == 0)
        return 0;
    
    for(i=0; i<2; i++)
    {
        
        for(j=i; j<strlen(str); j+=2)
            hash = (hash * 0x4D) + (str[j] - 0x2D);
    }
    
    return hash;
}


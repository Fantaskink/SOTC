#include "sdk/libcdvd.h"


extern u8 D_40049A68[3];
extern u8 D_40049A70[3];
extern void* D_40049A74;
extern void* D_40049A78;

void iosCdInit(void)
{
    sceCdInit(0);
    sceCdMmode(2);
    D_40049A68[0] = 0;
    D_40049A68[1] = 1;
    D_40049A68[2] = 0;
    D_40049A70[0] = 0;
    D_40049A70[1] = 0;
    D_40049A70[2] = 0;
    D_40049A74 = D_40049A78 = &D_40049A68;
}


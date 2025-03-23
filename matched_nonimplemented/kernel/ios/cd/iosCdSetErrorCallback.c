#include "stddef.h"

extern s32 D_400454B0;
extern s32 D_400454B4;

void iosCdSetErrorCallback(s32 arg0, s32 arg1)
{
    D_400454B0 = arg0;
    D_400454B4 = arg1;
}

#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosAllocIopHeap);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosFreeIopHeap);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosLoadIopModule);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosGetIopTotalFreeMemSize);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosGetIopTotalMemSize);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosWaitIopSema);

INCLUDE_ASM("asm/nonmatchings/ios/iop", iosSignalIopSema);

INCLUDE_ASM("asm/nonmatchings/ios/iop", InitIopDev);

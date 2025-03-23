#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosAllocIopHeap);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosFreeIopHeap);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosLoadIopModule);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosGetIopTotalFreeMemSize);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosGetIopTotalMemSize);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosWaitIopSema);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", iosSignalIopSema);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/iop", InitIopDev);

#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadTickProc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadGetXZInputL);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadGetXZInputLwithThreshold);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadGetXZInputLwithThresholdIndepAxis);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadGetXZInputR);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadEntryTickFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadRead);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadChkConnect);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pad", iosPadInit);

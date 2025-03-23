#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/print", iosSPrintf);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/print", iosUsbSerialPrintf);

void iosCreatePrintMgr(void) {
}

INCLUDE_ASM("asm/kernel/nonmatchings/ios/print", iosGetPrintfMode);

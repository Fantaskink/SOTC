#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/print", iosSPrintf);

INCLUDE_ASM("asm/nonmatchings/ios/print", iosUsbSerialPrintf);

void iosCreatePrintMgr(void) {
}

INCLUDE_ASM("asm/nonmatchings/ios/print", iosGetPrintfMode);

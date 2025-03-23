#include "common.h"

void iosInitDebPc(void) {
}

void iosStopPc(void) {
}

void iosDebPcVsyncCounterCheck(void) {
}

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pc", iosGetCnt0Pc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/pc", iosGetCnt1Pc);

void InitDebPc(void) {
}

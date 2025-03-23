#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/cd", func_400121D0);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosIopRead);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdStStart);

void iosCdStStop(void) {
}

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdStRead);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdStSkip);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdStSectorAlignSkip);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdSetErrorCallback);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdSetMode);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdStInit);

INCLUDE_ASM("asm/nonmatchings/ios/cd", iosCdInit);

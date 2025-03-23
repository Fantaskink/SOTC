#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/message", iosSendMsg);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/message", iosRecvMsg);

void iosInitMessageSystem(void) {
}

INCLUDE_ASM("asm/kernel/nonmatchings/ios/message", iosCreateMsgQueue);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/message", D_40016BB8);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/message", D_40016BD0);

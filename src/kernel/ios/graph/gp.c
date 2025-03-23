#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", InitGp);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosWaitDMA);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosWaitVU1);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", getCRTCoffsetU);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", getCRTCoffsetV);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosGsChangeScreenOffset);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSyncGP);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", bootGPThread);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosTransGP);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosChkGpDlTransExec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosGPFrameInit);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSetIStartFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSetGpFinishFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSetIFinishFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", getIntrGpCount);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSetDlTransFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosSetVSchedulerFunc);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosWaitGPFinish);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", D_40028600);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iStart);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iFinish);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", D_400286E8);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsRegBegin);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsRegEnd);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsRegEndWithPtr);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsReglistBegin);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsReglistEnd);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsBox);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsBox2);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsBoxLine);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", spriteNoTexFast);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", spriteTexFast);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", gsReg);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", getSpriteDrawFirst);

void gsReglist(void) {
}

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", iosTransTex);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/graph/gp", getTWH);

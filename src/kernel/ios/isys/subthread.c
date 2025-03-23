#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", _isysCreateSubMsgQueue);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", _isysRecvSubMsg);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", _isysSendSubMsg);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", InitSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysInitSubThreadExecEnv);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysStartSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysDeleteSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysDumpSubThreadStatus);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysWakeupSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", func_4002EAFC);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysSleepSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", SubThreadTerminateRestorePoint);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysGetThisSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysInitSubThreadObj);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysCreateSubThreadS);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysSleepSubThreadM);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysJumpSubThread);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", func_4002F17C);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysGetSubThreadWorkSize);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysGetSubThreadStackUse);

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysGetSubThreadStackSize);

void isysDumpSubThreadPC(void) {
}

INCLUDE_ASM("asm/nonmatchings/ios/isys/subthread", isysDebugSetSubThreadSleepProtection);

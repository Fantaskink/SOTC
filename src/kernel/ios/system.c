#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/system", iosInit);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosAddIntcHandler);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosCreateSema);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosDeleteSema);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosInitIntrSystem);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosGetTCount);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosGetTCountExtension);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosGetSecondFromTCount);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosResetCpuRapCounter);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosGetCpuRapCountPar1Int);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosDI);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosEI);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosLoaderResetEntry);

INCLUDE_ASM("asm/nonmatchings/ios/system", D_40013058);

INCLUDE_ASM("asm/nonmatchings/ios/system", D_40013090);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosEE2IopRequest);

INCLUDE_ASM("asm/nonmatchings/ios/system", iosEE2IopEndChk);

void iosCreateIopMgr(void) {
}

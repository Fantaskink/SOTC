#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMvecZero);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMvecAllOnes);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMvecAllOne);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosAddMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosClampSW_AddMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosClampUW_AddMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosSubMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMulMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosApplyM_Mat);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosCalcIcolorToBrightness);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosIColScale);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosIColAddVec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosSwClip255Mvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosIColAddVecXYZ);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosUcharColClampAdd);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosSWToMmColMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMmColToSWMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosITOF15ToMmColMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMmColToITOF15Mvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosFvecToMmColMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosMmColToFvecMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosSwClipMvec);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosFvecToICol);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosIColToMvecSl);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/math/mvec", iosIColToFvec);

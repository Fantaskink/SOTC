#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", GetStrHashWorkBuffer);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", InitStrHash);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", AddStrHashKey);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", MakeStrHashValue);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", SearchStrHashKey);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", ReleaseStrHashKeyAll);

INCLUDE_ASM("asm/kernel/nonmatchings/ios/hash", DeleteStrHashKey);

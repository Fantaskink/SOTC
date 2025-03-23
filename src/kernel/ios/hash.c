#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ios/hash", GetStrHashWorkBuffer);

INCLUDE_ASM("asm/nonmatchings/ios/hash", InitStrHash);

INCLUDE_ASM("asm/nonmatchings/ios/hash", AddStrHashKey);

INCLUDE_ASM("asm/nonmatchings/ios/hash", MakeStrHashValue);

INCLUDE_ASM("asm/nonmatchings/ios/hash", SearchStrHashKey);

INCLUDE_ASM("asm/nonmatchings/ios/hash", ReleaseStrHashKeyAll);

INCLUDE_ASM("asm/nonmatchings/ios/hash", DeleteStrHashKey);

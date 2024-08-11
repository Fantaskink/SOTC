#include "common.h"
#include "padSys.h"

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysTickProc);

INCLUDE_ASM("asm/nonmatchings/os/padSys", padsysInit);

struct t_padSysData *padSysGet(s32 padId)
{
    return &padSysData[padId];
}

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysReadForLoader);

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysVibSetAcrParam);

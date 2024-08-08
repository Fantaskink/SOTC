#include "common.h"
#include "padSys.h"

INCLUDE_ASM(const s32, "os/padSys", padSysTickProc);

INCLUDE_ASM(const s32, "os/padSys", padsysInit);

struct t_padSysData *padSysGet(int padId)
{
    return &padSysData[padId];
}

INCLUDE_ASM(const s32, "os/padSys", padSysReadForLoader);

INCLUDE_ASM(const s32, "os/padSys", padSysVibSetAcrParam);

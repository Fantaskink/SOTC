#include "common.h"
#include "sdk/ee/eekernel.h"

extern s32 D_0013A218;

INCLUDE_ASM("asm/nonmatchings/os/powerOff", LoaderSysSetPowerOffCallBackFunc);

INCLUDE_ASM("asm/nonmatchings/os/powerOff", PreparePowerOff);

INCLUDE_ASM("asm/nonmatchings/os/powerOff", PowerOffThread);

static void PowerOffHandler(void *arg)
{
    iSignalSema(D_0013A218);
}

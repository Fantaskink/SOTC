#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"

typedef void(cbFunc_t)(void);

extern s32 D_0013A218;
extern cbFunc_t *D_0013A21C;
extern const char D_0013A220[];
extern const char D_0013A228[];

INCLUDE_ASM("asm/nonmatchings/os/powerOff", LoaderSysSetPowerOffCallBackFunc);

INCLUDE_ASM("asm/nonmatchings/os/powerOff", PreparePowerOff);

void PowerOffThread(void *arg)
{
    s32 stat;

    while (1)
    {
        WaitSema(D_0013A218);

        if (D_0013A21C != NULL)
        {
            D_0013A21C();
        }

        PutStringS(0x4080FF80, "\npower off request has come.\n");

        // close all files
        sceDevctl(GSTR(D_0013A220, "pfs:"), PDIOC_CLOSEALL, NULL, 0, NULL, 0);

        // dev9 power off, need to power off PS2
        while (sceDevctl(GSTR(D_0013A228, "dev9x:"), DDIOC_OFF, NULL, 0, NULL, 0) < 0)
            ;

        // PS2 power off
        while (!sceCdPowerOff(&stat) || stat)
            ;
    }
}

static void PowerOffHandler(void *arg)
{
    iSignalSema(D_0013A218);
}

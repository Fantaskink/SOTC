#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"

#define STACKSZ 0x1000

typedef void(cbFunc_t)(void);

extern unsigned char D_0013DC40[STACKSZ] __attribute__((aligned(16)));
extern s32 D_0013A218;
extern cbFunc_t *D_0013A21C;
extern const char D_0013A220[];
extern const char D_0013A228[];

void LoaderSysSetPowerOffCallBackFunc(cbFunc_t *arg0);
void PreparePowerOff(void);
void PowerOffThread(void *arg);
static void PowerOffHandler(void *arg);

void LoaderSysSetPowerOffCallBackFunc(cbFunc_t *arg0)
{
    D_0013A21C = arg0;
}

void PreparePowerOff(void)
{
    struct SemaParam sparam;
    struct ThreadParam tparam;
    int tid;

    sparam.initCount = 0;
    sparam.maxCount = 1;
    sparam.option = 0;
    D_0013A218 = CreateSema(&sparam);

    // ChangeThreadPriority(GetThreadId(), 2);	//No such in SotC.

    tparam.stackSize = STACKSZ; // sizeof(stack)	//Original setting order actually matches the asm, while asm field write order does not.
    tparam.gpReg = &_gp;
    tparam.entry = PowerOffThread;
    tparam.stack = (void *)D_0013DC40;
    tparam.initPriority = 1;
    tid = CreateThread(&tparam);

    StartThread(tid, NULL);

    sceCdPOffCallback(PowerOffHandler, NULL);
}

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

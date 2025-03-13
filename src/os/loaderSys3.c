#include "loaderSys3.h"
#include "common.h"
#include "ee/libdbc.h"
#include "ee/sifrpc.h"
#include "gcc/stdio.h"
#include "gcc/stdlib.h"
#include "libcdvd.h"
#include "loaderSys.h"
#include "loaderSys2.h"
#include "loaderSysException.h"
#include "loaderSysFileIO.h"
#include "loaderSysMemprint.h"
#include "padSys.h"
#include "powerOff.h"
#include "putString.h"
#include "usbSerialSys.h"

s32 D_0013A110 = 0;
s32 D_0013A114 = 0;

void LoaderSysRebootIop(const char *arg0)
{
    PutString(PUTSTR_COL_CYAN, "\tRebooting Iop\n\t\twith ");
    PutString(PUTSTR_COL_TEALA, "\"%s\"", arg0);
    PutStringS(PUTSTR_COL_CYAN, "... ");

    while (sceSifRebootIop(arg0) == 0)
        ;

    while (sceSifSyncIop() == 0)
        ;

    PutStringS(PUTSTR_COL_CYAN, "Done.\n");
}

s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, const char *args)
{
    s32 result;

    PutString(PUTSTR_COL_LBLUE, "\t\tLoading ");
    PutString(PUTSTR_COL_LLBLUE, "\"%s\"", path);
    PutStringS(PUTSTR_COL_LBLUE, "... ");
    result = sceSifLoadModule(path, arg_count, args);
    if (result < 0)
    {
        PutStringS(PUTSTR_COL_LORANGE, "ERROR\n\t\t\tCouldn't load \"%s\".\n\t\t\t\t( Error code: %d )\n", path, result);
        return -1;
    }
    PutStringS(PUTSTR_COL_LBLUE, "Done.\n");
    return 0;
}

s32 LoaderSysUnloadIopModuleByName(const char *arg0, s32 arg1, s32 arg2, s32 *arg3)
{
    s32 modId;
    s32 success;
    s32 dummy;

    PutString(PUTSTR_COL_LBLUE, "\t\tUnloading ");
    PutString(PUTSTR_COL_LLBLUE, "\"%s\"", arg0);
    PutStringS(PUTSTR_COL_LBLUE, "... ");
    modId = sceSifSearchModuleByName(arg0);

    if (modId >= 0)
    {
        success = sceSifStopModule(modId, 1, "", arg3 ? arg3 : &dummy);
        if (success >= 0)
        {
            success = sceSifUnloadModule(modId);
        }
    }
    else
    {
        success = modId;
    }

    if (success < 0)
    {
        PutStringS(PUTSTR_COL_LORANGE, "ERROR\n\t\t\tCouldn't unload \"%s\".\n\t\t\t\t( Error code: %d )...\n", arg0, success);
        return -1;
    }

    PutStringS(PUTSTR_COL_LBLUE, "Done.\n");
    return success;
}

#define LOAD_MODULE(ident)                                                             \
    {                                                                                  \
        s32 result;                                                                    \
        result = LoaderSysLoadIopModule("cdrom0:\\MODULES\\" ident ".IRX;1", 0, NULL); \
        if (result < 0)                                                                \
        {                                                                              \
            while (1)                                                                  \
                ;                                                                      \
        }                                                                              \
        setNewIopIdentifier(ident);                                                    \
    }

void func_001033B0()
{
    sceSifInitRpc(0);
    sceSifInitIopHeap();

    PutString(PUTSTR_COL_WHITE, "Initialize loader (Version: ");
    // PutString(PUTSTR_COL_LGREEN, "%s %s", __DATE__, __TIME__);
    PutString(PUTSTR_COL_LGREEN, "%s %s", "Jul 12 2005", "16:20:22");
    PutStringS(PUTSTR_COL_WHITE, ")\n\n");
    PutStringS(PUTSTR_COL_LLBLUE, "\tWarm up CD/DVD hardware... ");
    sceCdInit(0);
    sceCdMmode(2);
    PutStringS(PUTSTR_COL_LLBLUE, "Done.\n");

    LoaderSysRebootIop("cdrom0:\\IOPRP300.IMG;1");

    sceSifInitRpc(0);
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(2);
    PreparePowerOff();
    sceSifLoadFileReset();
    sceFsReset();
    sceDmaReset(1);

    LOAD_MODULE("SIO2MAN");

    LOAD_MODULE("DBCMAN");

    LOAD_MODULE("SIO2D");

    LOAD_MODULE("DS1O_D");

    sceSifLoadFileReset();
    sceFsReset();
    sceDmaReset(1);
    sceDbcInit();
    padsysInit();

    LOAD_MODULE("USBD");

    LOAD_MODULE("PL2303");
    usbSerialSysInit();
}

s32 D_0013A178 = 0;                // Dummy
s32 LOADER_RESET_CALLBACK_NUM = 0; // 0x0013A17C
s32 D_0013A180 = 0;
s32 D_0013A184 = 0;

#define MAX_RESET_CALLBACKS 10
extern t_resetCallback RESET_CALLBACK_LIST[MAX_RESET_CALLBACKS];

s32 end;
static inline void loaderPrintMessage()
{
    void *heap_base;

    register int sp asm("sp");
    register int gp asm("gp");
    heap_base = (void *)malloc(1);
    free(heap_base);
    PutStringS(PUTSTR_COL_WHITE, " GP: %08p\n", gp);
    PutStringS(PUTSTR_COL_WHITE, " SP: %08p\n", sp);
    PutStringS(PUTSTR_COL_WHITE, " HEAP: %08p\n", heap_base);
    PutStringS(PUTSTR_COL_WHITE, " END: %08p\n\n", &end);
    PutStringS(PUTSTR_COL_WHITE, " LOADER HEAP START: %08p\n", D_0013A110);
    memset(&RESET_CALLBACK_LIST, 0, sizeof(RESET_CALLBACK_LIST));
    LOADER_RESET_CALLBACK_NUM = 0;
}

void loaderSetResetCallback(t_resetCallback callback)
{
    if (LOADER_RESET_CALLBACK_NUM == MAX_RESET_CALLBACKS)
    {
        LoaderSysPrintf("ld: ERROR LOADER_RESET_CALLBACK_NUM over \n");
        LOADER_RESET_CALLBACK_NUM = 0;
    }

    RESET_CALLBACK_LIST[LOADER_RESET_CALLBACK_NUM++] = callback;
}

void loaderExecResetCallback(void)
{
    s32 i;

    for (i = 0; i < MAX_RESET_CALLBACKS; i++)
    {
        if (RESET_CALLBACK_LIST[i] != NULL)
        {
            RESET_CALLBACK_LIST[i]();
        }
    }

    memset(&RESET_CALLBACK_LIST, 0, sizeof(RESET_CALLBACK_LIST));
    LOADER_RESET_CALLBACK_NUM = 0;
}

void loaderLoop(void)
{
    s32 reset_count;

    reset_count = 0;
    loaderPrintMessage();

    while (1)
    {
        SetHeapStartPoint(D_0013A110);
        D_0013A114 = execProgWithThread("cdrom0:\\STARTUP.XFF;1", 2);
        SleepThread();

        loaderExecResetCallback();
        reset_count++;
        LoaderSysExecuteRecoveryFirstProcess();
        ChangeThreadPriority(D_0013A184, 1);
        LoaderSysDeleteAllExternalIntcHandler();
        LoaderSysDeleteAllExternalThread();
        LoaderSysDeleteAllExternalSema();
        LoaderSysDeleteAllExternalIopMemory();
        ReinitDisp();
        PutStringS(PUTSTR_COL_WHITE, "\nreset: %d\n", reset_count);
    }
}

extern char D_0013D110[]; // Filled at runtime: "cdrom0:\SCPS_15"
s32 main(s32 argc, char **argv)
{
    register void *sp asm("sp");
    s32 i;
    s32 heap;
    s32 heap_size;

    printf("ld: start: argc:%d \n", argc);

    for (i = 0; i < argc; i++)
    {
        printf("ld: \targc %d: %s\n", i, argv[i]);
    }

    if (argc != 0)
    {
        strncpy(D_0013D110, argv[0], 16);
        D_0013D110[15] = 0;
    }
    else
    {
        D_0013D110[0] = 0;
    }

    D_0013A184 = GetThreadId();
    InitDisp();
    initmemprintf(0x2000000, 0x7ffff00);
    func_001033B0();

    heap = LoaderSysGetHeapBase();
    heap_size = LoaderSysGetHeapSize();
    D_0013A110 = ALIGN(heap + heap_size, 0x10);

    InitException();
    LoaderSysInitExternalIntcHandlerList();
    LoaderSysInitExternalThreadList();
    LoaderSysInitExternalSemaList();
    LoaderSysInitExternalIopMemoryList();
    ChangeThreadPriority(D_0013A184, 1);
    PutStringS(PUTSTR_COL_WHITE, "\nInitialize loader complete.\n\n\0");
    D_0013A180 = (s32)sp;
    loaderLoop();

    return 0;
}

char *LoaderSysGetBootArg(void)
{
    return D_0013D110;
}

void LoaderSysHookPoint(void)
{
}

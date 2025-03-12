#include "loaderSys2.h"
#include "common.h"
#include "fl_xfftype.h"
#include "gcc/string.h"
#include "putString.h"
#include "regnames.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sif.h"
#include "sdk/ee/sifdev.h"

extern char D_0013A100[]; // "host0:"

extern unk_except_s D_00131E00[14];
extern unk_except_s D_00131E80[2][32];

typedef struct
{
    union
    {
        u128 q;
        u64 d[2];
        u32 s[4];
    };
} qword;

extern struct unk_00131D00_s D_00131D00;

extern int D_0013A180;
extern int D_0013A184;


extern s32 PreparePowerOff(void);
extern void setNewIopIdentifier(const char *);
extern void padsysInit(void);
extern s32 usbSerialSysInit(void);

extern const char D_0013A138[];
extern const char D_0013A140[];
extern const char D_0013A148[];
extern const char D_0013A150[];
extern const char D_0013A158[];
extern const char D_0013A160[];
extern const char D_0013A168[];
extern const char D_0013A170[];

// TODO: Merge with LoaderSysRebootIop once the file is done
static inline void __inlined_LoaderSysRebootIop(const char *arg0)
{
    PutString(PUTSTR_COL_CYAN, "\tRebooting Iop\n\t\twith ");
    PutString(PUTSTR_COL_TEALA, GSTR(D_0013A118, "\"%s\""), arg0);
    PutStringS(PUTSTR_COL_CYAN, GSTR(D_0013A120, "... "));

    while (sceSifRebootIop(arg0) == 0)
        ;

    while (sceSifSyncIop() == 0)
        ;

    PutStringS(PUTSTR_COL_CYAN, GSTR(D_0013A128, "Done.\n"));
}

// TODO: Merge with LoaderSysLoadIopModule once the file is done
static inline s32 __inlined_LoaderSysLoadIopModule(const char *module, s32 arg_count, void *args)
{
    s32 result;

    PutString(PUTSTR_COL_LBLUE, "\t\tLoading ");
    PutString(PUTSTR_COL_LLBLUE, GSTR(D_0013A118, "\"%s\""), module);
    PutStringS(PUTSTR_COL_LBLUE, GSTR(D_0013A120, "... "));
    result = sceSifLoadModule(module, arg_count, args);
    if (result < 0)
    {
        PutStringS(PUTSTR_COL_LORANGE, "ERROR\n\t\t\tCouldn't load \"%s\".\n\t\t\t\t( Error code: %d )\n", module, result);
        return -1;
    }
    PutStringS(PUTSTR_COL_LBLUE, GSTR(D_0013A128, "Done.\n"));
    return 0;
}

// TODO: Remove once the file is done
static inline void __rodata_LoaderSysUnloadIopModuleByName()
{
    PutStringS(PUTSTR_COL_BLACK, "\t\tUnloading ");
    PutStringS(PUTSTR_COL_BLACK, "ERROR\n\t\t\tCouldn't unload \"%s\".\n\t\t\t\t( Error code: %d )...\n");
}


#define LOAD_MODULE(path, ident)                                  \
    {                                                             \
        s32 result;                                               \
        result = __inlined_LoaderSysLoadIopModule(path, 0, NULL); \
        if (result < 0)                                           \
        {                                                         \
            while (1)                                             \
                ;                                                 \
        }                                                         \
        setNewIopIdentifier(ident);                               \
    }

void func_001033B0()
{
    const char *module;
    s32 result;
    s32 r;

    sceSifInitRpc(0);
    sceSifInitIopHeap();

    PutString(PUTSTR_COL_WHITE, "Initialize loader (Version: ");
    // PutString(PUTSTR_COL_LGREEN, GSTR(D_0013A138, "%s %s"), __DATE__, __TIME__);
    PutString(PUTSTR_COL_LGREEN, GSTR(D_0013A138, "%s %s"), "Jul 12 2005", "16:20:22");
    PutStringS(PUTSTR_COL_WHITE, GSTR(D_0013A140, ")\n\n"));
    PutStringS(PUTSTR_COL_LLBLUE, "\tWarm up CD/DVD hardware... ");

    sceCdInit(0);
    sceCdMmode(2);

    PutStringS(PUTSTR_COL_LLBLUE, GSTR(D_0013A128, "Done.\n"));

    // TODO: Use normal LoaderSysRebootIop once the file is done
    __inlined_LoaderSysRebootIop("cdrom0:\\IOPRP300.IMG;1");

    sceSifInitRpc(0);
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(2);
    PreparePowerOff();
    sceSifLoadFileReset();
    sceFsReset();
    sceDmaReset(1);

    LOAD_MODULE("cdrom0:\\MODULES\\SIO2MAN.IRX;1", GSTR(D_0013A148, "SIO2MAN"));

    LOAD_MODULE("cdrom0:\\MODULES\\DBCMAN.IRX;1", GSTR(D_0013A150, "DBCMAN"));

    LOAD_MODULE("cdrom0:\\MODULES\\SIO2D.IRX;1", GSTR(D_0013A158, "SIO2D"));

    LOAD_MODULE("cdrom0:\\MODULES\\DS1O_D.IRX;1", GSTR(D_0013A160, "DS1O_D"));

    sceSifLoadFileReset();
    sceFsReset();
    sceDmaReset(1);
    sceDbcInit();
    padsysInit();

    LOAD_MODULE("cdrom0:\\MODULES\\USBD.IRX;1", GSTR(D_0013A168, "USBD"));

    LOAD_MODULE("cdrom0:\\MODULES\\PL2303.IRX;1", GSTR(D_0013A170, "PL2303"));
    usbSerialSysInit();
}

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

// TODO: Get rid of this once the file is done
const char D_00136E30[] = "ld: ERROR LOADER_RESET_CALLBACK_NUM over \n";

void loaderLoop(void)
{
    s32 reset_count;
    s32 i;

    reset_count = 0;
    loaderPrintMessage();

    while (1)
    {
        SetHeapStartPoint(D_0013A110);
        D_0013A114 = execProgWithThread("cdrom0:\\STARTUP.XFF;1", 2);
        SleepThread();

        // TODO: inlined call to loaderExecResetCallback
        {
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
    s32 r;
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

// TODO: Why is this needed?
char padding[8] __attribute__((section(".rodata")));

const char *LoaderSysGetBootArg(void)
{
    return D_0013D110;
}

s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, void *args)
{
    return __inlined_LoaderSysLoadIopModule(path, arg_count, args);
}

extern char D_0013A130[];
// s32 sceSifSearchModuleByName(char *name);                                 /* extern */
s32 sceSifStopModule(s32 modid, s32 args, const char *argp, s32 *result); /* extern */
s32 sceSifUnloadModule(s32);

s32 LoaderSysUnloadIopModuleByName(const char *arg0, s32 arg1, s32 arg2, s32 *arg3)
{
    s32 modId;
    s32 success;
    s32 dummy;

    PutString(PUTSTR_COL_LBLUE, "\t\tUnloading ");
    PutString(PUTSTR_COL_LLBLUE, GSTR(D_0013A118, "\"%s\""), arg0);
    PutStringS(PUTSTR_COL_LBLUE, GSTR(D_0013A120, "... "));
    modId = sceSifSearchModuleByName(arg0);

    if (modId >= 0)
    {
        success = sceSifStopModule(modId, 1, D_0013A130, arg3 ? arg3 : &dummy);
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

    PutStringS(PUTSTR_COL_LBLUE, GSTR(D_0013A128, "Done.\n"));
    return success;
}

void LoaderSysHookPoint(void)
{
}

extern s32 sceSifRebootIop(const char *);
extern s32 sceSifSyncIop(void);

void LoaderSysRebootIop(const char *arg0)
{
    __inlined_LoaderSysRebootIop(arg0);
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

void loaderSetResetCallback(t_resetCallback callback)
{
    if (LOADER_RESET_CALLBACK_NUM == MAX_RESET_CALLBACKS)
    {
        LoaderSysPrintf(GSTR(D_00136E30, "ld: ERROR LOADER_RESET_CALLBACK_NUM over \n"));
        LOADER_RESET_CALLBACK_NUM = 0;
    }

    RESET_CALLBACK_LIST[LOADER_RESET_CALLBACK_NUM++] = callback;
}

s32 memprintf(const char *in, ...)
{
}

s32 imemprintf(const char *in, ...)
{
}

void initmemprintf(s32 a, s32 b)
{
}

#include "common.h"

// Define macros for accessing the extern arrays
#define SEMAPHORE_LIST D_0013BD10
#define THREAD_LIST D_0013B910
#define IOP_MEMORY_LIST D_0013C910

#define SEMA_LIST_LEN 256
#define THREAD_LIST_LEN 256
#define IOP_MEM_LIST_LEN 256

extern s32 D_0013BD10[SEMA_LIST_LEN];
extern s32 D_0013B910[THREAD_LIST_LEN];
extern s32 D_0013C910[IOP_MEM_LIST_LEN];

struct unk
{
    s32 unk0;
    s32 unk4;
};

extern struct unk D_0013C110[256];

// Function prototypes
extern s32 GetThreadId();
extern s32 ChangeThreadPriority(s32 thread_id, s32 priority);
extern s32 sceMkdir(u8 *name, s32 mode);
extern s32 sceRmdir(const char *name);
extern s32 sceRead(s32 fd, void *buf, s32 count);
extern s32 sceWrite(s32 fd, const void *buf, s32 count);
extern s32 sceRemove(const char *name);
extern s32 sceLseek(s32 fd, s32 offset, s32 where);
extern s64 sceLseek64(s32 fd, s64 offset, s32 whence);
extern s32 sceOpen(const char *name, s32 flags);
extern s32 sceClose(s32 fd);

INCLUDE_ASM(const s32, "os/loadersys3", mallocAlignMempool);

INCLUDE_ASM(const s32, "os/loadersys3", mallocAlign0x100Mempool);

s32 _checkExistString(char *string, char **strings)
{
    s32 i = 0;

    while (strings[i] != 0)
    {
        if (strcmp(strings[i], string) == 0)
        {
            return 1;
        }
        i++;
    }

    return 0;
}

INCLUDE_ASM(const s32, "os/loadersys3", func_00101B88);

INCLUDE_ASM(const s32, "os/loadersys3", _execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys3", execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys3", func_00101EC0);

INCLUDE_ASM(const s32, "os/loadersys3", InitException);

INCLUDE_ASM(const s32, "os/loadersys3", setCop0Epc);

INCLUDE_ASM(const s32, "os/loadersys3", func_001021E0);

INCLUDE_ASM(const s32, "os/loadersys3", func_00102360);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteAllExternalIntcHandler);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteAllExternalSema);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysExecuteRecoveryFirstProcess);

static inline s32 getStA()
{
    s32 i;
    for (i = 0; i < 256; i++)
    {
        if (D_0013C110[i].unk4 < 0)
            return i;
    }
    return -1;
}

void LoaderSysEntryExternalIntcHandlerList(s32 param_1, s32 param_2)
{
    s32 i = getStA();
    D_0013C110[i].unk4 = param_1;
    D_0013C110[i].unk0 = param_2;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalThreadList);

static inline int findIndex(void)
{
    int i;
    for (i = 0; i < SEMA_LIST_LEN; i++)
    {
        if (SEMAPHORE_LIST[i] < 0)
        {
            return i;
        }
    }
    return -1;
}

void LoaderSysEntryExternalSemaList(s32 arg1)
{
    SEMAPHORE_LIST[findIndex()] = arg1;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteExternalIntcHandlerList);

s32 LoaderSysDeleteExternalThreadList(s32 thread_id)
{
    s32 i;

    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if (THREAD_LIST[i] == thread_id)
        {
            THREAD_LIST[i] = -1;
            return thread_id;
        }
    }
    return -1;
}

s32 LoaderSysDeleteExternalSemaList(s32 sema_id)
{
    s32 i;
    for (i = 0; i < SEMA_LIST_LEN; i++)
    {
        if (SEMAPHORE_LIST[i] == sema_id)
        {
            SEMAPHORE_LIST[i] = -1;
            return sema_id;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteExternalIopMemoryList);

void LoaderSysInitExternalIntcHandlerList(void)
{
    int i;

    for (i = 0; i < 256; i++)
    {
        D_0013C110[i].unk0 = D_0013C110[i].unk4 = -1;
    }
}

void LoaderSysInitExternalSemaList(void)
{
    s32 i;
    for (i = 0; i < SEMA_LIST_LEN; i++)
    {
        SEMAPHORE_LIST[i] = -1;
    }
}

void LoaderSysInitExternalThreadList(void)
{
    s32 i;
    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        THREAD_LIST[i] = -1;
    }
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysExternalThreadListCallBack);

void LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority)
{
    s32 i;
    s32 thread_id;

    thread_id = GetThreadId();
    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if ((THREAD_LIST[i] != thread_id) && (-1 < THREAD_LIST[i]))
        {
            ChangeThreadPriority(THREAD_LIST[i], priority);
        }
    }
    return;
}

void LoaderSysDeleteAllExternalThread(void)
{
    s32 i;

    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if (THREAD_LIST[i] >= 0)
        {
            TerminateThread(THREAD_LIST[i]);
            DeleteThread(THREAD_LIST[i]);
            THREAD_LIST[i] = -1;
        }
    }
}

void LoaderSysDeleteAllExternalThreadExceptMe(void)
{
    s32 thread_id;
    s32 i;

    thread_id = GetThreadId();

    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if ((THREAD_LIST[i] != thread_id) && (THREAD_LIST[i] >= 0))
        {
            TerminateThread(THREAD_LIST[i]);
            DeleteThread(THREAD_LIST[i]);
            THREAD_LIST[i] = -1;
        }
    }
}

void LoaderSysInitExternalIopMemoryList(void)
{
    s32 i;

    for (i = 0; i < IOP_MEM_LIST_LEN; i++)
    {
        IOP_MEMORY_LIST[i] = 0;
    }
}

void LoaderSysDeleteAllExternalIopMemory(void)
{
    s32 i;

    sceSifInitRpc(0);
    sceSifInitIopHeap();

    for (i = 0; i < IOP_MEM_LIST_LEN; i++)
    {
        if (IOP_MEMORY_LIST[i] != 0)
        {
            sceSifFreeIopHeap(IOP_MEMORY_LIST[i]);
            IOP_MEMORY_LIST[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysPrintf);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysLoadIopModuleFromEEBuffer);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysCheckCDBootMode);

extern const char D_0013A0F8[];
extern s32 printf(const char *string, ...);

void LoaderSysPutString(char *string)
{
    printf(D_0013A0F8, string);
}

s32 LoaderSysFOpen(const char *name, s32 flags)
{
    s32 success = sceOpen(name, flags);
    return success;
}

s32 LoaderSysFClose(s32 fd)
{
    s32 success = sceClose(fd);
    return success;
}

s32 LoaderSysFSeek(s32 fd, s32 offset, s32 whence)
{
    s32 success = sceLseek(fd, offset, whence);
    return success;
}

s64 LoaderSysFSeek64(s32 fd, s64 offset, s32 whence)
{
    s64 success = sceLseek64(fd, offset, whence);
    return success;
}

s32 LoaderSysFRead(s32 fd, void *buf, s32 count)
{
    s32 success = sceRead(fd, buf, count);
    return success;
}

s32 LoaderSysFWrite(s32 fd, const void *buf, s32 count)
{
    s32 success = sceWrite(fd, buf, count);
    return success;
}

s32 LoaderSysRemove(const char *name)
{
    s32 success = sceRemove(name);
    return success;
}

s32 LoaderSysRmdir(const char *name)
{
    s32 success = sceRmdir(name);
    return success;
}

s32 LoaderSysMkdir(u8 *name, s32 mode)
{
    s32 success = sceMkdir(name, mode);
    return success;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysGetstat);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysChstat);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysGetMemoryInfo);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDumpIopModuleIdentifiers);

void LoaderSysSendAbort(void)
{
}

void LoaderSysFlushPrint(void)
{
}

INCLUDE_ASM(const s32, "os/loadersys3", checkHookDesc);

INCLUDE_ASM(const s32, "os/loadersys3", setNewIopIdentifier);

INCLUDE_ASM(const s32, "os/loadersys3", func_001033B0);

INCLUDE_ASM(const s32, "os/loadersys3", loaderLoop);

INCLUDE_ASM(const s32, "os/loadersys3", main);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysGetBootArg);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysLoadIopModule);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysUnloadIopModuleByName);

void LoaderSysHookPoint(void)
{
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysRebootIop);

INCLUDE_ASM(const s32, "os/loadersys3", loaderExecResetCallback);

INCLUDE_ASM(const s32, "os/loadersys3", loaderSetResetCallback);

INCLUDE_ASM(const s32, "os/loadersys3", memprintf);

INCLUDE_ASM(const s32, "os/loadersys3", imemprintf);

void initmemprintf(void)
{
}

INCLUDE_ASM(const s32, "os/loadersys3", func_00104090);

INCLUDE_ASM(const s32, "os/loadersys3", func_00104428);

INCLUDE_ASM(const s32, "os/loadersys3", func_00104668);

INCLUDE_ASM(const s32, "os/loadersys3", func_00104818);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysInitTCP);

INCLUDE_ASM(const s32, "os/loadersys3", padSysTickProc);

INCLUDE_ASM(const s32, "os/loadersys3", padsysInit);

INCLUDE_ASM(const s32, "os/loadersys3", padSysGet);

INCLUDE_ASM(const s32, "os/loadersys3", padSysReadForLoader);

INCLUDE_ASM(const s32, "os/loadersys3", padSysVibSetAcrParam);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysSetPowerOffCallBackFunc);

INCLUDE_ASM(const s32, "os/loadersys3", PreparePowerOff);

INCLUDE_ASM(const s32, "os/loadersys3", func_00104FF0);

INCLUDE_ASM(const s32, "os/loadersys3", usbSerialSysPutString);

INCLUDE_ASM(const s32, "os/loadersys3", usbSerialSysPrintf);

INCLUDE_ASM(const s32, "os/loadersys3", usbSerialSysInit);

INCLUDE_ASM(const s32, "os/loadersys3", func_001053F0);

INCLUDE_ASM(const s32, "os/loadersys3", PutFont);

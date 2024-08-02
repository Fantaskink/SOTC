#include "common.h"

#define SEMA_LIST_LEN 256
#define THREAD_LIST_LEN 256

extern s32 D_0013BD10[SEMA_LIST_LEN];   // sema_list
extern s32 D_0013B910[THREAD_LIST_LEN]; // thread_list

// Function prototypes
s32 GetThreadId();
s32 ChangeThreadPriority(s32 thread_id, s32 priority);

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

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalThreadList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalSemaList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysEntryExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteExternalIntcHandlerList);

s32 LoaderSysDeleteExternalThreadList(s32 thread_id)
{
    s32 i;

    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if (D_0013B910[i] == thread_id)
        {
            D_0013B910[i] = -1;
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
        if (D_0013BD10[i] == sema_id)
        {
            D_0013BD10[i] = -1;
            return sema_id;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysInitExternalIntcHandlerList);

void LoaderSysInitExternalSemaList(void)
{
    s32 i;
    for (i = 0; i < SEMA_LIST_LEN; i++)
    {
        D_0013BD10[i] = -1;
    }
}

void LoaderSysInitExternalThreadList(void)
{
    s32 i;
    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        D_0013B910[i] = -1;
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
        if ((D_0013B910[i] != thread_id) && (-1 < D_0013B910[i]))
        {
            ChangeThreadPriority(D_0013B910[i], priority);
        }
    }
    return;
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteAllExternalThread);

void LoaderSysDeleteAllExternalThreadExceptMe(void)
{
    s32 thread_id;
    s32 i;

    thread_id = GetThreadId();

    for (i = 0; i < THREAD_LIST_LEN; i++)
    {
        if ((D_0013B910[i] != thread_id) && (D_0013B910[i] >= 0))
        {
            TerminateThread(D_0013B910[i]);
            DeleteThread(D_0013B910[i]);
            D_0013B910[i] = -1;
        }
    }
}

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysInitExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysDeleteAllExternalIopMemory);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysPrintf);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysLoadIopModuleFromEEBuffer);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysCheckCDBootMode);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysPutString);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFOpen);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFClose);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFSeek);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFSeek64);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFRead);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysFWrite);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysRemove);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysRmdir);

INCLUDE_ASM(const s32, "os/loadersys3", LoaderSysMkdir);

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

INCLUDE_ASM(const s32, "os/loadersys3", _putString);

INCLUDE_ASM(const s32, "os/loadersys3", PutString);

INCLUDE_ASM(const s32, "os/loadersys3", PutStringS);

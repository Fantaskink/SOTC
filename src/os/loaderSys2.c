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

void LoaderSysDeleteAllExternalIntcHandler(void)
{
    s32 i;

    for (i = 0; i < 0x100; i++)
    {
        if (D_0013C110[i].unk4 >= 0)
        {
            DisableIntc(D_0013C110[i].unk0);
            RemoveIntcHandler(D_0013C110[i].unk0, D_0013C110[i].unk4);
            D_0013C110[i].unk0 = D_0013C110[i].unk4 = -1;
        }
    }

    __inlined_LoaderSysInitExternalIntcHandlerList();
}

extern u32 D_00132080[3];
static inline s32 LoaderSysCheckSemaAttribueList(s32 sema_id)
{
    struct SemaParam sparam;
    s32 i;

    if (ReferSemaStatus(sema_id, &sparam) == sema_id)
    {
        for (i = 0; i < 3; i++)
        {
            if (D_00132080[i] == sparam.attr)
            {
                return 1;
            }
        }
    }
    return 0;
}

void LoaderSysDeleteAllExternalSema(void)
{
    s32 i;

    for (i = 0; i < 0x100; i++)
    {
        if (SEMAPHORE_LIST[i] >= 0)
        {
            DeleteSema(SEMAPHORE_LIST[i]);
            SEMAPHORE_LIST[i] = -1;
        }
    }

    __inlined_LoaderSysInitExternalSemaList();

    for (i = 0; i < 0x100; i++)
    {
        if (LoaderSysCheckSemaAttribueList(i) != 0)
        {
            DeleteSema(i);
        }
    }
}

void LoaderSysExecuteRecoveryFirstProcess(void)
{
    ChangeThreadPriority(GetThreadId(), 1);
    LoaderSysDeleteAllExternalIntcHandler();
    __inlined_LoaderSysChangeExternalThreadPriorityExceptMe(0x7F);
    __inlined_LoaderSysDeleteAllExternalThreadExceptMe();
    LoaderSysDeleteAllExternalSema();
    LoaderSysFlushPrint();
    __inlined_LoaderSysDeleteAllExternalIopMemory();
}

static inline s32 getStA()
{
    s32 i;
    for (i = 0; i < MAX_INTC_HANDLERS; i++)
    {
        if (INTC_HANDLER_LIST[i].unk4 < 0)
            return i;
    }
    return -1;
}

void LoaderSysEntryExternalIntcHandlerList(s32 param_1, s32 param_2)
{
    s32 i = getStA();
    INTC_HANDLER_LIST[i].unk4 = param_1;
    INTC_HANDLER_LIST[i].unk0 = param_2;
}

static inline s32 FindThreadIndex(void)
{
    s32 i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        if (THREAD_LIST[i] < 0)
        {
            return i;
        }
    }
    return -1;
}

void LoaderSysEntryExternalThreadList(s32 thread_id)
{
    THREAD_LIST[FindThreadIndex()] = thread_id;
}

static inline s32 FindSemaIndex(void)
{
    s32 i;
    for (i = 0; i < MAX_SEMAPHORES; i++)
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
    SEMAPHORE_LIST[FindSemaIndex()] = arg1;
}

static inline s32 FindIopMemoryIndex(s32 iopmem_id)
{
    s32 i;

    for (i = 0; i < IOP_MEM_LIST_LEN; i++)
    {
        if (IOP_MEMORY_LIST[i] == iopmem_id)
        {
            return i;
        }
    }
    return -1;
}

s32 LoaderSysEntryExternalIopMemoryList(s32 iopmem_id)
{
    s32 i;
    if (iopmem_id != 0)
    {
        i = FindIopMemoryIndex(0);

        if (i >= 0)
        {
            IOP_MEMORY_LIST[i] = iopmem_id;
            return i;
        }
    }

    return -1;
}

s32 LoaderSysDeleteExternalIntcHandlerList(s32 arg)
{
    s32 i;
    for (i = 0; i < MAX_INTC_HANDLERS; i++)
    {
        if (INTC_HANDLER_LIST[i].unk4 == arg)
        {
            INTC_HANDLER_LIST[i].unk0 = INTC_HANDLER_LIST[i].unk4 = -1;
            return arg;
        }
    }
    return -1;
}

s32 LoaderSysDeleteExternalThreadList(s32 threadId)
{
    s32 i;

    for (i = 0; i < MAX_THREADS; i++)
    {
        if (THREAD_LIST[i] == threadId)
        {
            THREAD_LIST[i] = -1;
            return threadId;
        }
    }
    return -1;
}

s32 LoaderSysDeleteExternalSemaList(s32 semaId)
{
    s32 i;
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
        if (SEMAPHORE_LIST[i] == semaId)
        {
            SEMAPHORE_LIST[i] = -1;
            return semaId;
        }
    }
    return -1;
}

s32 LoaderSysDeleteExternalIopMemoryList(s32 iopmem_id)
{
    s32 i = FindIopMemoryIndex(iopmem_id);

    if (i >= 0)
    {
        IOP_MEMORY_LIST[i] = 0;
    }

    return i;
}

void LoaderSysInitExternalIntcHandlerList(void)
{
    __inlined_LoaderSysInitExternalIntcHandlerList();
}

void LoaderSysInitExternalSemaList(void)
{
    __inlined_LoaderSysInitExternalSemaList();
}

void LoaderSysInitExternalThreadList(void)
{
    s32 i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        THREAD_LIST[i] = -1;
    }
}

void LoaderSysExternalThreadListCallBack(void (*fun_ptr)(s32))
{
    s32 i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        if (THREAD_LIST[i] != -1)
        {
            (*fun_ptr)(THREAD_LIST[i]);
        }
    }
}

void LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority)
{
    __inlined_LoaderSysChangeExternalThreadPriorityExceptMe(priority);
}

void LoaderSysDeleteAllExternalThread(void)
{
    s32 i;

    for (i = 0; i < MAX_THREADS; i++)
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
    __inlined_LoaderSysDeleteAllExternalThreadExceptMe();
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
    __inlined_LoaderSysDeleteAllExternalIopMemory();
}

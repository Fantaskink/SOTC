#ifndef LOADERSYS2_H
#define LOADERSYS2_H

#include "common.h"
#include "fl_xfftype.h"
#include "gcc/string.h"
#include "include_asm.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"
#include "sdk/ee/sifrpc.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_WHITE "\x1b[37m"
#define ANSI_RESET "\x1b[m"

struct unk
{
    s32 unk0;
    s32 unk4;
};

void LoaderSysDeleteAllExternalIntcHandler(void);
void LoaderSysDeleteAllExternalSema(void);
void LoaderSysExecuteRecoveryFirstProcess(void);
void LoaderSysEntryExternalIntcHandlerList(s32 param_1, s32 param_2);
void LoaderSysEntryExternalThreadList(s32 thread_id);
void LoaderSysEntryExternalSemaList(s32 arg1);
s32 LoaderSysEntryExternalIopMemoryList(s32 iopmem_id);
s32 LoaderSysDeleteExternalIntcHandlerList(s32 arg);
s32 LoaderSysDeleteExternalThreadList(s32 threadId);
s32 LoaderSysDeleteExternalSemaList(s32 semaId);
s32 LoaderSysDeleteExternalIopMemoryList(s32 iopmem_id);
void LoaderSysInitExternalIntcHandlerList(void);
void LoaderSysInitExternalSemaList(void);
void LoaderSysInitExternalThreadList(void);
void LoaderSysExternalThreadListCallBack(void (*fun_ptr)(s32));
void LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority);
void LoaderSysDeleteAllExternalThread(void);
void LoaderSysDeleteAllExternalThreadExceptMe(void);
void LoaderSysInitExternalIopMemoryList(void);
void LoaderSysDeleteAllExternalIopMemory(void);

#define SEMAPHORE_LIST D_0013BD10
#define THREAD_LIST D_0013B910
#define IOP_MEMORY_LIST D_0013C910
#define INTC_HANDLER_LIST D_0013C110

#define MAX_INTC_HANDLERS 256
#define IOP_MEM_LIST_LEN 256

extern s32 D_0013BD10[MAX_SEMAPHORES];
extern s32 D_0013B910[MAX_THREADS];
extern s32 D_0013C910[IOP_MEM_LIST_LEN];
extern struct unk D_0013C110[MAX_INTC_HANDLERS];

extern s32 D_0013A110;
extern s32 D_0013A184;
extern s32 D_0013A114;

// sdata externs
extern const char D_00131DB8[]; // "XFF2"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

// TODO: merge these
static inline void __inlined_LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority)
{
    s32 i;
    s32 threadId;

    threadId = GetThreadId();
    for (i = 0; i < MAX_THREADS; i++)
    {
        if ((THREAD_LIST[i] != threadId) && (-1 < THREAD_LIST[i]))
        {
            ChangeThreadPriority(THREAD_LIST[i], priority);
        }
    }
    return;
}

static inline void __inlined_LoaderSysDeleteAllExternalThreadExceptMe(void)
{
    s32 threadId;
    s32 i;

    threadId = GetThreadId();

    for (i = 0; i < MAX_THREADS; i++)
    {
        if ((THREAD_LIST[i] != threadId) && (THREAD_LIST[i] >= 0))
        {
            TerminateThread(THREAD_LIST[i]);
            DeleteThread(THREAD_LIST[i]);
            THREAD_LIST[i] = -1;
        }
    }
}

static inline void __inlined_LoaderSysDeleteAllExternalIopMemory(void)
{
    s32 i;

    sceSifInitRpc(0);
    sceSifInitIopHeap();

    for (i = 0; i < IOP_MEM_LIST_LEN; i++)
    {
        if (IOP_MEMORY_LIST[i] != 0)
        {
            sceSifFreeIopHeap((void *)IOP_MEMORY_LIST[i]);
            IOP_MEMORY_LIST[i] = 0;
        }
    }
}

static inline void __inlined_LoaderSysInitExternalSemaList(void)
{
    s32 i;
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
        SEMAPHORE_LIST[i] = -1;
    }
}

static inline void __inlined_LoaderSysInitExternalIntcHandlerList(void)
{
    s32 i;

    for (i = 0; i < MAX_INTC_HANDLERS; i++)
    {
        INTC_HANDLER_LIST[i].unk0 = INTC_HANDLER_LIST[i].unk4 = -1;
    }
}

#endif /* LOADERSYS2_H */

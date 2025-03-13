#ifndef LOADERSYS2_H
#define LOADERSYS2_H

#include "common.h"
#include "sdk/ee/eekernel.h"

struct unk
{
    s32 unk0;
    s32 unk4;
};

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
void LoaderSysDeleteAllExternalIntcHandler(void);
void LoaderSysDeleteAllExternalSema(void);

void LoaderSysInitExternalIopMemoryList(void);
void LoaderSysDeleteAllExternalIopMemory(void);

void LoaderSysExecuteRecoveryFirstProcess(void);

#endif /* LOADERSYS2_H */

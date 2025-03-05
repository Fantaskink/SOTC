#ifndef LOADERSYS2_H
#define LOADERSYS2_H

#include "include_asm.h"
#include "common.h"
#include "fl_xfftype.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"
#include "sdk/ee/sifrpc.h"
#include "gcc/string.h"

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_WHITE "\x1b[37m"
#define ANSI_RESET "\x1b[m"

extern void* D_00139F04; // heap pointer
#define HEAP_START D_00139F04
#define MAX_RESET_CALLBACKS 10

struct unk
{
    s32 unk0;
    s32 unk4;
};

typedef struct unk_stack_40 {
    struct t_xffEntPntHdr* unk0;
    int unk4;
    struct t_xffRelocEnt *unk8;
    void* unkC;
} unk_stack_40;

typedef struct unk_00131D00_s {
    char path[0x40];
    s32 stack_size;
    struct t_xffEntPntHdr* unk44;
} unk_00131D00_s;

typedef struct memory_info {
    void* stack_base;
    void* stack_end;
    void* heap_base;
    void* heap_end;
    void* module_stack_base;
    void* module_stack_end;
    void* unk18;
    void* stack_base2;
    unk_00131D00_s module_info;
} memory_info;

typedef struct unk_except_s {
    u32 value;
    char* name;
} unk_except_s;

typedef (*t_resetCallback)();
typedef int (dispose_reloc_func)(struct t_xffEntPntHdr*, struct t_xffRelocAddrEnt*, unk_stack_40*);
typedef void*(mallocAlign_func)(s32, u32);
typedef void*(mallocMaxAlign_func)(s32);
typedef int (ldrDbgPrintf_func)(const char *, ...);

void func_00101EC0(s32 except_code, u32 cause, u32 epc, u32 bva, u32 bpa, unk_except_s* ctx, u32 mode);
void InitException(void);
void setCop0Epc(int epc);
void func_001021E0(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128* gpr);
void func_00102360(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128* gpr);
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
s32 LoaderSysPrintf(const char *format, ...);
s32 LoaderSysLoadIopModuleFromEEBuffer(const char* module_ident, s32 data, s32 module_size, s32 argc, const char* argp);
s32 LoaderSysCheckCDBootMode();
void LoaderSysPutString(char *string);
s32 LoaderSysFOpen(const char *name, s32 flags, s32 mode);
s32 LoaderSysFClose(s32 fd);
s32 LoaderSysFSeek(s32 fd, s32 offset, s32 whence);
s64 LoaderSysFSeek64(s32 fd, s64 offset, s32 whence);
s32 LoaderSysFRead(s32 fd, void *buf, s32 count);
s32 LoaderSysFWrite(s32 fd, const void *buf, s32 count);
s32 LoaderSysRemove(const char *name);
s32 LoaderSysRmdir(const char *name);
s32 LoaderSysMkdir(u8 *name, s32 mode);
s32 LoaderSysGetstat(const char *name, struct sce_stat *buf);
s32 LoaderSysChstat(const char *name, struct sce_stat *buf, u32 cbit);
s32 LoaderSysGetMemoryInfo(memory_info* info);
void LoaderSysDumpIopModuleIdentifiers(void);
void LoaderSysSendAbort(void);
void LoaderSysFlushPrint(void);
char *checkHookDesc(char *hook_desc);
void setNewIopIdentifier(const char *newIdentifier);
void func_001033B0();
void loaderLoop(void);
s32 main(s32 argc, char** argv);
const char *LoaderSysGetBootArg(void);
s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, void *args);
s32 LoaderSysUnloadIopModuleByName(const char *arg0, s32 arg1, s32 arg2, s32 *arg3);
void LoaderSysHookPoint(void);
void LoaderSysRebootIop(const char *arg0);
void loaderExecResetCallback(void);
void loaderSetResetCallback(t_resetCallback callback);
s32 memprintf(const char *in, ...);
s32 imemprintf(const char *in, ...);
void initmemprintf(s32 a, s32 b);

extern void PutString(s32, const char *, ...);
extern void PutStringS(s32, const char *, ...);

extern s32 LOADER_RESET_CALLBACK_NUM;
extern t_resetCallback RESET_CALLBACK_LIST[MAX_RESET_CALLBACKS];

#define SEMAPHORE_LIST D_0013BD10
#define THREAD_LIST D_0013B910
#define IOP_MEMORY_LIST D_0013C910
#define INTC_HANDLER_LIST D_0013C110

#define MAX_INTC_HANDLERS 256
#define IOP_MEM_LIST_LEN 256
#define MAX_IOP_IDENTIFIERS 64

extern s32 D_0013BD10[MAX_SEMAPHORES];
extern s32 D_0013B910[MAX_THREADS];
extern s32 D_0013C910[IOP_MEM_LIST_LEN];
extern struct unk D_0013C110[MAX_INTC_HANDLERS];

extern s32 D_0013A108;        // number of iop modules
extern char D_0013CD10[MAX_IOP_IDENTIFIERS][16]; // iop module identifiers

extern s32 D_0013A110;
extern s32 D_0013A184;
extern s32 D_0013A114;

// rodata externs
extern const char D_00136200[]; // "ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n"
extern char D_00136A80[]; // "ldsys: setNewIopIdentifier: set new iop identifier \"%s\" at #%d\n", Referenced in LoaderSysLoadIopModuleFromEEBuffer

// sdata externs
extern const char D_00131DB8[]; // "XFF2"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

// Legitimate Static inlines
static inline int LoaderSysGetStackBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_stack)\n" "nop\n"
        "addiu %0,%%lo(_stack)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetStackSize() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_stack_size)\n" "nop\n"
        "addiu %0,%%lo(_stack_size)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetHeapBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_end)\n" "nop\n"
        "addiu %0,%%lo(_end)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetHeapSize() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_heap_size)\n" "nop\n"
        "addiu %0,%%lo(_heap_size)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

// TODO: merge these
static inline void __inlined_LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority) {
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

static inline void __inlined_setNewIopIdentifier(const char* newIdentifier) {
    LoaderSysPrintf(D_00136A80, newIdentifier, D_0013A108);
    strncpy(D_0013CD10[D_0013A108++], newIdentifier, strlen(newIdentifier));
}

#endif /* LOADERSYS2_H */

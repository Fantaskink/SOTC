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

s32 LoaderSysPrintf(const char *format, ...)
{
    va_list args;
    char buffer[256];

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    printf(buffer);

    return strlen(buffer);
}

static inline s32 LoaderSysSearchInLoadedIopModules(const char *module_name)
{
    s32 i;
    for (i = 0; i < D_0013A108; i++)
    {
        if (!strcmp((const char *)&D_0013CD10[i], module_name))
        {
            return i;
        }
    }
    return -1;
}

extern const char D_00136A00[];
extern const char D_00136A48[];
static inline s32 _loadIopModuleFromEEBuffer(s32 data, s32 module_size, s32 argc, const char *argp)
{
    s32 stat;
    void *module_addr;
    s32 queue_id;
    sceSifDmaData dma_data;

    module_size = ALIGN(module_size, 4);

    sceSifInitRpc(0x0);
    sceSifInitIopHeap();

    module_addr = sceSifAllocSysMemory(0x1, module_size, NULL);
    LoaderSysPrintf(GSTR(D_00136A00, "ldsys: _loadIopModuleFromEEBuffer: allocated iop memory:%p(size:%d)\n"), module_addr, module_size);
    FlushCache(0x0);
    dma_data.addr = (int)module_addr;
    dma_data.mode = 0x0;
    dma_data.data = data;
    dma_data.size = module_size;
    queue_id = sceSifSetDma(&dma_data, 0x1);

    while (-0x1 < sceSifDmaStat(queue_id))
        ;

    LoaderSysPrintf(GSTR(D_00136A48, "ldsys: _loadIopModuleFromEEBuffer: senddma: finish\n"));
    stat = sceSifLoadModuleBuffer(module_addr, argc, argp);
    sceSifFreeSysMemory(module_addr);
    return stat;
}

extern const char D_00136AC0[];
s32 LoaderSysLoadIopModuleFromEEBuffer(const char *module_ident, s32 data, s32 module_size, s32 argc, const char *argp)
{
    s32 stat;
    s32 module_id;

    if (module_ident != 0x0)
    {
        module_id = LoaderSysSearchInLoadedIopModules(module_ident);
        if (module_id < 0x0)
        {
            stat = _loadIopModuleFromEEBuffer(data, module_size, argc, argp);
            if (-0x1 < stat)
            {
                __inlined_setNewIopIdentifier(module_ident);
            }
        }
        else
        {
            LoaderSysPrintf(GSTR(D_00136AC0, "ldsys: LoaderSysLoadIopModuleFromEEBuffer: iop identifier \"%s\" exists at #%d (load skipped).\n"), module_ident, module_id);
            return 0x0;
        }
    }
    else
    {
        stat = _loadIopModuleFromEEBuffer(data, module_size, argc, argp);
    }
    return stat;
}

s32 LoaderSysCheckCDBootMode()
{
    return 2;
}
extern const char D_0013A0F8[]; // "%s", .sdata
void LoaderSysPutString(char *string)
{
    printf(GSTR(D_0013A0F8, "%s"), string);
}

s32 LoaderSysFOpen(const char *name, s32 flags, s32 mode)
{
    s32 success = sceOpen(name, flags, mode);
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

s32 LoaderSysGetstat(const char *name, struct sce_stat *buf)
{
    return sceGetstat(name, buf);
}

s32 LoaderSysChstat(const char *name, struct sce_stat *buf, u32 cbit)
{
    return sceChstat(name, buf, cbit);
}

extern s32 D_00139F00;
s32 LoaderSysGetMemoryInfo(memory_info *info)
{
    u32 stack_size;
    u32 heap_size;

    info->unk18 = (void *)D_00139F00;

    info->stack_base = (void *)LoaderSysGetStackBase();
    stack_size = LoaderSysGetStackSize();
    info->heap_base = (void *)LoaderSysGetHeapBase();
    heap_size = LoaderSysGetHeapSize();

    info->module_stack_base = info->module_stack_end = (void *)D_0013A114;
    info->module_stack_end += D_00131D00.stack_size;
    info->stack_base2 = info->stack_base;
    info->stack_end = info->stack_base + stack_size;
    info->heap_end = info->heap_base + heap_size;
    info->module_info = D_00131D00;

    return 1;
}

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136A00);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136A48);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136A80);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136AC0);

void LoaderSysDumpIopModuleIdentifiers(void)
{
    s32 i;

    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: dump list of iop identifiers\n");
    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: ----------------------------\n");

    for (i = 0; i < D_0013A108; i++)
    {
        LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: \t% 2d: \"%s\"\n", i, D_0013CD10[i]);
    }

    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: ----------------------------\n");
}

void LoaderSysSendAbort(void)
{
}

void LoaderSysFlushPrint(void)
{
}

char *checkHookDesc(char *hook_desc)
{
    s32 hasDiff;
    char *desc;

    hasDiff = strncmp(hook_desc, GSTR(D_0013A100, "host0:"), 6);
    desc = hook_desc + 6;
    if (hasDiff == 0)
    {
        return desc;
    }
    return NULL;
}

void setNewIopIdentifier(const char *newIdentifier)
{
    LoaderSysPrintf(D_00136A80, newIdentifier, D_0013A108);
    strncpy(D_0013CD10[D_0013A108++], newIdentifier, strlen(newIdentifier));
}

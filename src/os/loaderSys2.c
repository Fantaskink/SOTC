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

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136630);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136640);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136660);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136680);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001366A0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001366B8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001366D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001366F8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136718);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136738);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136768);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136788);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001367B8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001367D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001367E0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_001367F0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136800);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136810);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136820);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136830);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys2", D_00136840);

extern const char D_0013A0F0[];
extern qword D_0013B900;

// TODO: Get rid of this

const char D_00136850[] = "     %s:%08x %s:%08x %s:%08x %s:%08x \n";

void func_00101EC0(s32 except_code, u32 cause, u32 epc, u32 bva, u32 bpa, unk_except_s *ctx, u32 mode)
{
    s32 i;
    s32 j;

    switch (mode)
    {
    case 0:
        PutString(PUTSTR_COL_YELLOW, "      %s\n\n", D_00131E00[except_code].name);
        PutString(PUTSTR_COL_TEAL, "   pc:%08x ", epc);
        PutString(PUTSTR_COL_LBLUE2, "bva:%08x bpa:%08x ", bva, bpa);
        PutString(PUTSTR_COL_GRAY, "cause:%08x\n", cause);
        PutString(PUTSTR_COL_WHITE, GSTR(D_0013A0F0, "\n"));

        // dump registers
        for (j = 0; j < 8; j++)
        {
            PutString(PUTSTR_COL_LLBLUE2, GSTR(D_00136850, "     %s:%08x %s:%08x %s:%08x %s:%08x \n"),
                      ctx[(j * 4) + 0].name, ctx[(j * 4) + 0].value,
                      ctx[(j * 4) + 1].name, ctx[(j * 4) + 1].value,
                      ctx[(j * 4) + 2].name, ctx[(j * 4) + 2].value,
                      ctx[(j * 4) + 3].name, ctx[(j * 4) + 3].value);
        }
        break;

    case 1:
        for (i = 0; i < 16; i++)
        {
            D_0013B900.q = ctx[i].value;
            PutString(PUTSTR_COL_WHITE, "%s=%8.8x_%8.8x_%8.8x_%8.8x\n", ctx[i].name,
                      D_0013B900.s[3], D_0013B900.s[2],
                      D_0013B900.s[1], D_0013B900.s[0]);
        }
        break;

    case 2:
        for (i = 0; i < 16; i++)
        {
            D_0013B900.q = ctx[i + 0x10].value;
            PutString(PUTSTR_COL_WHITE, "%s=%8.8x_%8.8x_%8.8x_%8.8x\n", ctx[i + 0x10].name,
                      D_0013B900.s[3], D_0013B900.s[2],
                      D_0013B900.s[1], D_0013B900.s[0]);
        }
        break;
    }
}

void InitException(void)
{
    register void *sp asm("sp");
    s32 i;

    for (i = 0; i < 14u; i++)
    {
        switch (i)
        {
        case 0: // No exception
        case 8: // System call exception
            break;
        case 9: // Breakpoint exception
            SetDebugHandler(D_00131E00[i].value, func_00102360);
            break;
        default: // Other exceptions
            SetDebugHandler(D_00131E00[i].value, func_001021E0);
            break;
        }
    }

    // TODO: This works, but maybe the array is 1D?
    for (i = 0; i < 64; i++)
    {
        D_00131E80[0][i].value = 0;
    }

    D_00131E80[0][MIPS_REG_SP].value = (u32)sp;
    D_00131E80[0][MIPS_REG_RA].value = (u32)&main;
    D_00131E80[1][COP0_REG_EPC].value = (u32)&main;
}

void setCop0Epc(int epc)
{
    asm(
        "mtc0 %0, $%1\n" ::"r"(epc), "i"(COP0_REG_EPC));
}

void func_001021E0(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128 *gpr)
{
    register void *gp asm("gp");
    s32 th_id;
    s32 except_code;
    s32 i;

    except_code = COP0_CAUSE_GET_EXCEPT_CODE(cause);
    th_id = GetThreadId();
    gp = &_gp;

    LoaderSysExecuteRecoveryFirstProcess();
    ChangeThreadPriority(th_id, 0x7f);
    EIntr();
    LoaderSysPrintf(ANSI_YELLOW "exception" ANSI_RESET ": exceptional abort.(threadid:%d)\n", th_id);

    for (i = 0; i < 32; i++)
    {
        D_00131E80[0][i].value = gpr[i];
        D_00131E80[1][i].value = iGetCop0(i);
    }

    D_00131E80[1][COP0_REG_CAUSE].value = cause;
    D_00131E80[1][COP0_REG_EPC].value = epc;

    PutString(PUTSTR_COL_WHITE, GSTR(D_0013A0F0, "\n"));
    PutString(PUTSTR_COL_LORANGE2, "     -----------------------------------------------\n");
    PutString(PUTSTR_COL_LORANGE2, "           ios reports critical error message.\n");
    PutString(PUTSTR_COL_LORANGE2, "     -----------------------------------------------\n");
    PutString(PUTSTR_COL_ORANGE, GSTR(D_0013A0F0, "\n"));
    func_00101EC0(except_code, cause, epc, bva, bpa, (unk_except_s *)&D_00131E80, 0x0);
    LoaderSysJumpRecoverPointNoStateSetting("recovering from exception...\n");
}

void func_00102360(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128 *gpr)
{
    register void *gp asm("gp");
    s32 th_id;
    s32 i;

    th_id = GetThreadId();
    gp = &_gp;

    LoaderSysExecuteRecoveryFirstProcess();
    ChangeThreadPriority(th_id, 0x7F);
    EIntr();
    LoaderSysPrintf(ANSI_YELLOW "exception" ANSI_RESET ": break instruction appeared.(threadid:%d)\n", th_id);

    for (i = 0; i < 32; i++)
    {
        D_00131E80[0][i].value = gpr[i];
        D_00131E80[1][i].value = iGetCop0(i);
    }

    D_00131E80[1][COP0_REG_CAUSE].value = cause;
    D_00131E80[1][COP0_REG_EPC].value = epc;

    LoaderSysJumpRecoverPointNoStateSetting("recovering from break...\n");
}

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

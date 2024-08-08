#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"
#include "gcc/string.h"
#include "fl_xfftype.h"

extern char *D_00131DC0[]; // {{"normal use"}, {"\x1B[36mout of align(alloc)\x1B[m"}, {"alloc flag(alloc)"}}
extern char D_00136318[];  // "ld:\t\tdecode section\n"
extern char D_00136330[];  // "ld:\t%15s(progbit): 0x%08x(0x%08x) %s\n"
extern char D_00136358[];  // "ld:\t%15s(overlaydata): 0x%08x(0x%08x) %s\n"
extern char D_00136388[];  // "ld:\t%15s(nobit)  : 0x%08x(0x%08x) %s\n"

extern u32 D_00139F04; // heap pointer

#define HEAP_START D_00139F04

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

extern char D_0013A100[]; // "host0:"

extern const char D_0013D110[]; // Filled at runtime: "cdrom0:\SCPS_15"
extern const char D_0013A0F8[]; // "%s"

struct unk
{
    s32 unk0;
    s32 unk4;
};

// Function prototypes
extern s32 printf(const char *string, ...);

INCLUDE_ASM(const s32, "os/loadersys", ResolveRelocation);

void DecodeSection(
    void *xffBuf,
    void *(*mallocAlign)(int sz, int align),
    void *(*mallocMaxAlign)(int sz),
    void (*ldrDbgPrintf)(char *fmt, ...))
{
    int i;
    struct t_xffEntPntHdr *xffEp;
    void *entPntSectBs = NULL;
    struct t_xffSsNmOffs *nmOffs;
    struct t_xffSectEnt *sect;

    xffEp = xffBuf;

    i = xffEp->sectNrE - 1;
    sect = &xffEp->sectTab[1];
    nmOffs = &xffEp->ssNamesOffs[0];

    if (ldrDbgPrintf != NULL)
        ldrDbgPrintf(GSTR(D_00136318, "ld:\t\tdecode section\n"));

    // The zero section is not processed as it is all 0.
    for (; i--; sect++, nmOffs++)
    {

        sect->moved = 0;
        if (sect->size != 0)
        {
            switch (sect->type)
            {
            case 1:
            case 0x7FFFF420: // .text section
                if (sect->flags != 0)
                {
                    // Forced max alignment
                    sect->memPt = mallocMaxAlign(sect->size);
                    memcpy(sect->memPt, sect->filePt, sect->size);
                    sect->moved = 2;
                }
                else
                {
                    // Check if the alignment is enough as is in the file:
                    if (((u32)sect->filePt & (sect->align - 1)) != 0)
                    {
                        // Insufficient alignment, so alloicate
                        sect->memPt = mallocAlign(sect->size, sect->align);
                        memcpy(sect->memPt, sect->filePt, sect->size);
                        sect->moved = 1;
                    }
                    else
                    { // use the section as is in the file
                        sect->memPt = sect->filePt;
                    }
                }
                if (ldrDbgPrintf != NULL)
                {
                    if (sect->type == 1)
                    { // .text
                        // The moved-types are actually allocation types (or used in file as is).
                        ldrDbgPrintf(GSTR(D_00136330, "ld:\t%15s(progbit): 0x%08x(0x%08x) %s\n"), &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                    }
                    else
                    { // VU code/data
                        // The moved-types are actually allocation types (or used in file as is).
                        ldrDbgPrintf(GSTR(D_00136358, "ld:\t%15s(overlaydata): 0x%08x(0x%08x) %s\n"), &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                    }
                }
                break;
            case 8: // nobit .bss section
                // Because nobits is not present in the file, it is always allocated
                if (sect->flags != 0)
                { // section is not copied and used as is in the file
                    sect->memPt = mallocMaxAlign(sect->size);
                    sect->moved = 2;
                }
                else
                {
                    sect->memPt = mallocAlign(sect->size, sect->align);
                    sect->moved = 1;
                }

                memset(sect->memPt, 0x00, sect->size);
                if (ldrDbgPrintf != NULL)
                {
                    // The moved-types are actually allocation types (or used in file as is).
                    ldrDbgPrintf(GSTR(D_00136388, "ld:\t%15s(nobit)  : 0x%08x(0x%08x) %s\n"), &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                }
                break;
            }
        }
        else
        {
            sect->memPt = NULL;
        }

        if (entPntSectBs == NULL)
            entPntSectBs = sect->memPt;
    }

    xffEp->entryPnt = (void *)((u32)entPntSectBs + xffEp->entryPnt_Rel);
    return;
}

INCLUDE_ASM(const s32, "os/loadersys", RelocateElfInfoHeader);

INCLUDE_ASM(const s32, "os/loadersys", OutputLinkerScriptFile);

INCLUDE_ASM(const s32, "os/loadersys", func_00100A58);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysJumpRecoverPointNoStateSetting);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysJumpRecoverPoint);

INCLUDE_ASM(const s32, "os/loadersys", func_00100D48);

INCLUDE_ASM(const s32, "os/loadersys", MoveElf);

INCLUDE_ASM(const s32, "os/loadersys", func_001013C8);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRelocateOnlineElfInfo);

INCLUDE_ASM(const s32, "os/loadersys", RelocateCode);

INCLUDE_ASM(const s32, "os/loadersys", FreeDecodedSection);

INCLUDE_ASM(const s32, "os/loadersys", RelocateSelfSymbol);

INCLUDE_ASM(const s32, "os/loadersys", DisposeRelocationElement);

void SetHeapStartPoint(u32 start_address)
{
    HEAP_START = (start_address + 0xF) & ~0xF;
}

int GetHeapCurrentPoint(void)
{
    return HEAP_START;
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysResetSystem);

INCLUDE_ASM(const s32, "os/loadersys", mallocAlignMempool);

INCLUDE_ASM(const s32, "os/loadersys", mallocAlign0x100Mempool);

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

INCLUDE_ASM(const s32, "os/loadersys", func_00101B88);

INCLUDE_ASM(const s32, "os/loadersys", _execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys", execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys", func_00101EC0);

INCLUDE_ASM(const s32, "os/loadersys", InitException);

INCLUDE_ASM(const s32, "os/loadersys", setCop0Epc);

INCLUDE_ASM(const s32, "os/loadersys", func_001021E0);

INCLUDE_ASM(const s32, "os/loadersys", func_00102360);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalIntcHandler);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalSema);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysExecuteRecoveryFirstProcess);

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

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalThreadList);

static inline int FindSemaIndex(void)
{
    int i;
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

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalIopMemoryList);

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

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteExternalIopMemoryList);

void LoaderSysInitExternalIntcHandlerList(void)
{
    int i;

    for (i = 0; i < MAX_INTC_HANDLERS; i++)
    {
        INTC_HANDLER_LIST[i].unk0 = INTC_HANDLER_LIST[i].unk4 = -1;
    }
}

void LoaderSysInitExternalSemaList(void)
{
    s32 i;
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
        SEMAPHORE_LIST[i] = -1;
    }
}

void LoaderSysInitExternalThreadList(void)
{
    s32 i;
    for (i = 0; i < MAX_THREADS; i++)
    {
        THREAD_LIST[i] = -1;
    }
}

void LoaderSysExternalThreadListCallBack(void (*fun_ptr)(int))
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
            sceSifFreeIopHeap((void *)IOP_MEMORY_LIST[i]);
            IOP_MEMORY_LIST[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysPrintf);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysLoadIopModuleFromEEBuffer);

s32 LoaderSysCheckCDBootMode()
{
    return 2;
}

void LoaderSysPutString(char *string)
{
    printf(GSTR(D_0013A0F8, "%s"), string);
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

s32 LoaderSysGetstat(const char *name, struct sce_stat *buf)
{
    return sceGetstat(name, buf);
}

s32 LoaderSysChstat(const char *name, struct sce_stat *buf, u32 cbit)
{
    return sceChstat(name, buf, cbit);
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysGetMemoryInfo);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDumpIopModuleIdentifiers);

void LoaderSysSendAbort(void)
{
}

void LoaderSysFlushPrint(void)
{
}

char *checkHookDesc(char *hook_desc)
{
    int hasDiff;
    char *desc;

    hasDiff = strncmp(hook_desc, GSTR(D_0013A100, "host0:"), 6);
    desc = hook_desc + 6;
    if (hasDiff == 0)
    {
        return desc;
    }
    return NULL;
}

INCLUDE_ASM(const s32, "os/loadersys", setNewIopIdentifier);

INCLUDE_ASM(const s32, "os/loadersys", func_001033B0);

INCLUDE_ASM(const s32, "os/loadersys", loaderLoop);

INCLUDE_ASM(const s32, "os/loadersys", main);

const char *LoaderSysGetBootArg(void)
{
    return D_0013D110;
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysLoadIopModule);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysUnloadIopModuleByName);

void LoaderSysHookPoint(void)
{
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRebootIop);

INCLUDE_ASM(const s32, "os/loadersys", loaderExecResetCallback);

INCLUDE_ASM(const s32, "os/loadersys", loaderSetResetCallback);

INCLUDE_ASM(const s32, "os/loadersys", memprintf);

INCLUDE_ASM(const s32, "os/loadersys", imemprintf);

void initmemprintf(void)
{
}

INCLUDE_ASM(const s32, "os/loadersys", func_00104090);

INCLUDE_ASM(const s32, "os/loadersys", func_00104428);

INCLUDE_ASM(const s32, "os/loadersys", func_00104668);

INCLUDE_ASM(const s32, "os/loadersys", func_00104818);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitTCP);

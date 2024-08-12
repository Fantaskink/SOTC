#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"
#include "gcc/string.h"
#include "fl_xfftype.h"

extern char *D_00131DC0[]; // {{"normal use"}, {"\x1B[36mout of align(alloc)\x1B[m"}, {"alloc flag(alloc)"}}

extern char D_0013A100[];       // "host0:"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

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

struct unk
{
    s32 unk0;
    s32 unk4;
};

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136200);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", ResolveRelocation);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001362D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001362E8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136308);

void DecodeSection(
    void *xffBuf,
    void *(*mallocAlign)(s32 sz, s32 align),
    void *(*mallocMaxAlign)(s32 sz),
    void (*ldrDbgPrintf)(char *fmt, ...))
{
    s32 i;
    struct t_xffEntPntHdr *xffEp;
    void *entPntSectBs = NULL;
    struct t_xffSsNmOffs *nmOffs;
    struct t_xffSectEnt *sect;

    xffEp = xffBuf;

    i = xffEp->sectNrE - 1;
    sect = &xffEp->sectTab[1];
    nmOffs = &xffEp->ssNamesOffs[0];

    if (ldrDbgPrintf != NULL)
        ldrDbgPrintf("ld:\t\tdecode section\n");

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
                        ldrDbgPrintf("ld:\t%15s(progbit): 0x%08x(0x%08x) %s\n", &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                    }
                    else
                    { // VU code/data
                        // The moved-types are actually allocation types (or used in file as is).
                        ldrDbgPrintf("ld:\t%15s(overlaydata): 0x%08x(0x%08x) %s\n", &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
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
                    ldrDbgPrintf("ld:\t%15s(nobit)  : 0x%08x(0x%08x) %s\n", &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", RelocateElfInfoHeader);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001363B0);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", OutputLinkerScriptFile);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00100A58);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysJumpRecoverPointNoStateSetting);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysJumpRecoverPoint);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00100D48);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001364A8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001364D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136500);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136548);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136560);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136588);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", MoveElf);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_001013C8);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysRelocateOnlineElfInfo);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", RelocateCode);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", FreeDecodedSection);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", RelocateSelfSymbol);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", DisposeRelocationElement);

void SetHeapStartPoint(u32 start_address)
{
    HEAP_START = (start_address + 0xF) & ~0xF;
}

s32 GetHeapCurrentPoint(void)
{
    return HEAP_START;
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysResetSystem);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", mallocAlignMempool);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", mallocAlign0x100Mempool);

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00101B88);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", _execProgWithThread);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", execProgWithThread);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136630);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136640);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136660);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136680);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001366A0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001366B8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001366D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001366F8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136718);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136738);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136768);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136788);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001367B8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001367D0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001367E0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001367F0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136800);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136810);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136820);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136830);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136840);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00101EC0);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", InitException);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", setCop0Epc);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_001021E0);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00102360);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysDeleteAllExternalIntcHandler);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysDeleteAllExternalSema);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysExecuteRecoveryFirstProcess);

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysEntryExternalThreadList);

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysEntryExternalIopMemoryList);

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysDeleteExternalIopMemoryList);

void LoaderSysInitExternalIntcHandlerList(void)
{
    s32 i;

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysPrintf);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysLoadIopModuleFromEEBuffer);

s32 LoaderSysCheckCDBootMode()
{
    return 2;
}
extern const char D_0013A0F8[]; // "%s", .sdata
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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysGetMemoryInfo);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A00);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A48);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A80);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136AC0);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysDumpIopModuleIdentifiers);

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", setNewIopIdentifier);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_001033B0);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", loaderLoop);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136BE8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136C00);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136C10);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136C48);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136C58);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136C98);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136CB8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136CC8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136CD8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136CF8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136D10);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136D30);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136D50);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136D70);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136D90);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136DB0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136DD0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136DE0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136DF0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136E00);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136E10);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136E30);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136E60);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136E78);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", main);

extern const char D_0013D110[]; // Filled at runtime: "cdrom0:\SCPS_15"
extern const char D_00136C10[];
extern const char D_00136C00[];
const char *LoaderSysGetBootArg(void)
{
    return D_0013D110;
}

s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, void *args)
{
    s32 result;
    PutString(0x4080FF00, GSTR(D_00136C00, "\t\tLoading "));
    PutString(0x80C0FF00, GSTR(D_0013A118, "\"%s\""), path);
    PutStringS(0x4080FF00, GSTR(D_0013A120, "... "));

    result = sceSifLoadModule(path, arg_count, args);

    if (result < 0)
    {
        PutStringS(0xFF804000, GSTR(D_00136C10, "ERROR\n\t\t\tCouldn't load \"%s\".\n\t\t\t\t( Error code: %d )\n"), path, result);
        return -1;
    }

    PutStringS(0x4080FF00, GSTR(D_0013A128, "Done.\n"));
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysUnloadIopModuleByName);

void LoaderSysHookPoint(void)
{
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysRebootIop);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", loaderExecResetCallback);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", loaderSetResetCallback);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", memprintf);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", imemprintf);

void initmemprintf(void)
{
}

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136EE0); // INET.IRX

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136F00); // NETCNF.IRX

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136F20); // icon=cdrom

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136F68);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136F88);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136FA0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136FC0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136FE0);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137000);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137020);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137040);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137060);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137078);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137098);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001370B8);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001370D8);

// These are likely const char* (string literals)
extern const char D_00136F20[]; // "icon=cdrom"
extern const char D_00136F88[];
extern const char D_001370B8[];
extern const char D_001370D8[];

// These are likely also const char*, used for identifiers
extern const char D_0013A188[];
extern const char D_0013A190[];
extern const char D_0013A198[];
extern const char D_0013A1A0[];
extern const char D_0013A1A8[];
extern const char D_0013A1B0[];
extern const char D_0013A1B8[];
extern const char D_0013A1C0[];
extern const char D_0013A1C8[];
extern const char D_0013A1D0[];
extern const char D_0013A1D8[];
extern const char D_0013A1E0[];

// These are likely also const char*, used for identifiers
extern const char D_00136EE0[];
extern const char D_00136F00[];
extern const char D_00136F68[];
extern const char D_00136FA0[];
extern const char D_00136FC0[];
extern const char D_00136FE0[];
extern const char D_00137000[];
extern const char D_00137020[];
extern const char D_00137040[];
extern const char D_00137060[];
extern const char D_00137078[];
extern const char D_00137098[];

extern void setNewIopIdentifier(const char *identifier);

int func_00104090(int mode)
{
    if (LoaderSysLoadIopModule(D_00136EE0, 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A188);

    if (LoaderSysLoadIopModule(D_00136F00, 0x46, D_00136F20) < 0)
        return -1;
    setNewIopIdentifier(D_0013A190);

    if (LoaderSysLoadIopModule(D_00136F68, 0x14, D_00136F88) < 0)
        return -1;
    setNewIopIdentifier(D_0013A198);

    switch (mode)
    {
    case 1:
    case 2:
        if (LoaderSysLoadIopModule(D_00136FA0, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A0);
        if (LoaderSysLoadIopModule(D_00136FC0, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A8);
        break;
    case 5:
        if (LoaderSysLoadIopModule(D_00136FA0, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A0);
        break;
    case 3:
    case 4:
        if (LoaderSysLoadIopModule(D_00136FE0, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1B0);
        if (LoaderSysLoadIopModule(D_00137000, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1B8);
        break;
    case 0:
        break;
    }

    switch (mode)
    {
    case 2:
    case 4:
        if (LoaderSysLoadIopModule(D_00137020, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C0);
        if (LoaderSysLoadIopModule(D_00137040, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C8);
        break;
    case 5:
        if (LoaderSysLoadIopModule(D_00137020, 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C0);
        if (LoaderSysLoadIopModule(D_00137060, 1, D_0013A1D0) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1D0);
        break;
    case 0:
        LoaderSysLoadIopModule(D_00136FA0, 0, NULL);
        LoaderSysLoadIopModule(D_00136FC0, 0, NULL);
        LoaderSysLoadIopModule(D_00136FE0, 0, NULL);
        LoaderSysLoadIopModule(D_00137000, 0, NULL);
        LoaderSysLoadIopModule(D_00137020, 0, NULL);
        LoaderSysLoadIopModule(D_00137040, 0, NULL);
        break;
    }

    if (LoaderSysLoadIopModule(D_00137078, 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A1D8);

    if (LoaderSysLoadIopModule(D_00137098, 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A1E0);

    if (LoaderSysLoadIopModule(D_001370B8, 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_001370D8);

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00104428);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137130);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137140);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137150);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137160);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00137170);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00104668);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", func_00104818);

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysInitTCP);

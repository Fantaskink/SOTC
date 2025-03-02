#include "common.h"
#include "fl_xfftype.h"
#include "gcc/string.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"

#define R_MIPS_NONE (0)
#define R_MIPS_16 (1)
#define R_MIPS_32 (2)
#define R_MIPS_REL32 (3)
#define R_MIPS_26 (4)
#define R_MIPS_HI16 (5)
#define R_MIPS_LO16 (6)

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_RESET "\x1b[m"

extern char *D_00131DC0[]; // {{"normal use"}, {"\x1B[36mout of align(alloc)\x1B[m"}, {"alloc flag(alloc)"}}

extern char D_0013A100[];       // "host0:"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

extern u32 D_00139F04; // heap pointer
#define HEAP_START D_00139F04
#define MAX_RESET_CALLBACKS 10

typedef (*t_resetCallback)();
extern s32 LOADER_RESET_CALLBACK_NUM;
extern t_resetCallback RESET_CALLBACK_LIST[MAX_RESET_CALLBACKS];

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

s32 LoaderSysPrintf(const char *format, ...);
extern s32 PutString(s32, const char *, ...);
void LoaderSysSendAbort(void);
void loaderExecResetCallback(void);

struct unk
{
    s32 unk0;
    s32 unk4;
};

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136200);

static inline s32 SearchRelocLoPair(struct t_xffRelocEnt* relocEntry, s32 relocIndex) {
    s32 i;

    for (i = relocIndex + 1;; i++) {
        struct t_xffRelocAddrEnt *addr = &relocEntry->addr[i];
        switch (addr->tyIx & 0xFF) {
            case R_MIPS_HI16:
                // According to the ABI specs HI16 relocs *should* be followed by their corresponding LO16. 
                // HOWEVER, a common GNU extension allows having multiple consecute HI16 before a LO16
                // so we just skip consecutive HI16 until the next LO16...
                break;
            case R_MIPS_LO16:
                // Found a LO16!
                return relocEntry->inst[i].inst;
            default:
                // We found an unrelated reloc type, log a warning and return 0
                LoaderSysPrintf(
                    "ld:\t" ANSI_RED "Warning! Can't find low16 for hi16(relid:%d)." ANSI_RESET "\n", relocIndex
                );
                return 0;
        }
    }
}

s32 ResolveRelocation(void* xffBuf, struct t_xffRelocEnt* relocEnt, s32 relocIx) {
    struct t_xffEntPntHdr* xffEp;
    s32 relOffs;
    u32* relAddr;
    s32 addVal;
    void* tgtAddr;
    s32 loInstr;
    s32 a;
    s32 relTy;
    struct t_xffRelocAddrEnt *addr = &relocEnt->addr[relocIx];

    xffEp = xffBuf;
    relOffs = addr->addr;
    relTy = addr->tyIx & 0xFF;
    relAddr = (void*)((u32)xffEp->sectTab[relocEnt->sect].memPt + relOffs);
    tgtAddr = xffEp->symTab[addr->tyIx >> 8].addr;
    addVal = relocEnt->inst[relocIx].inst;

    switch (relTy) {
        case R_MIPS_NONE:
            // MIPS_NONE relocation
            // There's nothing to do
            break;

        case R_MIPS_32:
            // MIPS_32 relocation
            // Symbol shifted by the addend
            *relAddr = addVal + (u32)tgtAddr;
            break;

        case R_MIPS_26:
            // MIPS_26 relocation
            // Used in jal / j reloc with target symbol in symTab
            *relAddr = addVal + (((u32)tgtAddr / 4) & 0x03FFFFFF);
            break;

        case R_MIPS_HI16:
            // MIPS_HI16 relocation 
            // High part of immediate lui+addiu pair

            // Let's look for the paired LO16 relocation so that we can compute 
            // the high part correctly accounting for the signed offsets.            
            loInstr = SearchRelocLoPair(relocEnt, relocIx);
            
            // LO16 and HI16 instructions are of the form:
            // bit 31                   15                      0
            //     |--------------------------------------------|
            //     |     INSTRUCTION     |  MIPS_HI/LO16 VALUE  |
            //     |--------------------------------------------|
            
            // As the addend we want is the sign extended pairing of the HI16 and LO16
            // values we can just do: value = (HI16Instr << 0x10) + (short)LO16Instr
            a = (u32)tgtAddr + (addVal << 16) + (s16)loInstr;

            // Now, we are relocating the HI16 part, so extract it here
            // minding the sign-extension
            a >>= 15; a++; a >>= 1;
            *relAddr = (addVal & 0xFFFF0000) | (a & 0x0000FFFF);
            break;

        case R_MIPS_LO16:
            // MIPS_LO16 relocation
            // Lower part of immediate lui+addiu pair
            // reloc with target symbol in symTab
            *relAddr = (addVal & 0xFFFF0000) | (((u32)tgtAddr + addVal) & 0x0000FFFF);
            break;

        default:
            LoaderSysPrintf("ld:\tFatal error!! unknown relocation type(%d) appeared. (ofs:%x)\n", relTy, relOffs);
            return 0; // fail
    };

    return 1; // success
}

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

void RelocateElfInfoHeader(struct t_xffEntPntHdr* xffEp) {
    s32 i;
    u32 xffBaseAddr = (u32)xffEp;

    xffEp->sectTab = (void*)(xffBaseAddr + xffEp->sectTab_Rel);

    for (i = 0; i < xffEp->sectNrE; i++) {
        xffEp->sectTab[i].filePt = (void*)(xffBaseAddr + xffEp->sectTab[i].offs_Rel);
    }

    xffEp->symTab = (void*)(xffBaseAddr + xffEp->symTab_Rel);
	xffEp->symRelTab = (void*)(xffBaseAddr + xffEp->symRelTab_Rel);
	xffEp->symTabStr = (void*)(xffBaseAddr + xffEp->symTabStr_Rel);
	xffEp->impSymIxs = (void*)(xffBaseAddr + xffEp->impSymIxs_Rel);
	xffEp->relocTab = (void*)(xffBaseAddr + xffEp->relocTab_Rel);
	xffEp->ssNamesBase = (void*)(xffBaseAddr + xffEp->ssNamesBase_Rel);
	xffEp->ssNamesOffs = (void*)(xffBaseAddr + xffEp->ssNamesOffs_Rel);

    for (i = 0; i < xffEp->relocTabNrE; i++) {
        xffEp->relocTab[i].addr = (void*)(xffBaseAddr + xffEp->relocTab[i].addr_Rel);
		xffEp->relocTab[i].inst = (void*)(xffBaseAddr + xffEp->relocTab[i].inst_Rel);
    }

    xffEp->stack = (void*)(xffBaseAddr + xffEp->stack_Rel);
}

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_001363B0);

// char* D_00131DD0[8] = { ".text", ".vutext", ".data", ".vudata", ".rodata", ".bss", ".vubss", ".DVP.ovlytab" };
extern char* D_00131DD0[8];
extern u32 D_00139F40;
extern char D_00139F48[];
extern s32 D_0013A300;
char D_0013A4C8[0x1000];

// TODO: remove this
static inline s32 __inline_checkExistString(char *string, char **strings)
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

static inline void LoaderSysFWriteString(s32 fd, char* str, s32 flush) {
    s32 len;

    if (D_0013A300 == fd) {
        len = strlen(str);

        // Would we go over the buffer with this write? Flush to file
        if ((0xfff < (D_00139F40 + len)) && (D_00139F40 != 0x0)) {
            LoaderSysFWrite(D_0013A300, &D_0013A4C8, D_00139F40);
            D_00139F40 = 0x0;
        }

        // If the string to write is bigger than the buffer write directly
        // copy to the buffer otherwise
        if (strlen(str) >= 0x1000) {
            LoaderSysFWrite(D_0013A300, str, strlen(str));
        } else {
            strncpy(&D_0013A4C8[D_00139F40], str, strlen(str));
            D_00139F40 += strlen(str);
        }

        // If the flush flag is set then flush everything at the end
        if ((flush) && (D_0013A300 == fd) && (D_00139F40 != 0x0)) {
            LoaderSysFWrite(D_0013A300, &D_0013A4C8, D_00139F40);
            D_00139F40 = 0x0;
        }
    }
}

s32 OutputLinkerScriptFile(struct t_xffEntPntHdr* xffEp, char* ld_script_path, void (*callback)(void*)) {
    char buffer[1000];
    char pad[16];
    s32 linker_fd;
    s32 symbol;
    s32 section;

    if (callback != NULL) {
        callback("ld:\toutput linker script " ANSI_GREEN "\"%s\"" ANSI_RESET ".\n");
    }
    
    D_0013A300 = linker_fd = LoaderSysFOpen(ld_script_path, SCE_CREAT | SCE_TRUNC | SCE_WRONLY, 0);
    
    // Symbol writing might be an inline of its own
    for (symbol = 0; symbol < xffEp->impSymIxsNrE; symbol++) {
        sprintf(
            &buffer, "%s = 0x%08x;\n", xffEp->symTabStr + xffEp->symTab[xffEp->impSymIxs[symbol].stIx].nameOffs,
            xffEp->symTab[xffEp->impSymIxs[symbol].stIx].addr
        );
        LoaderSysFWriteString(linker_fd, (char*)&buffer, 0);
    }

    // Section writing might be an inline of its own
    LoaderSysFWriteString(linker_fd, "SECTIONS {\n", 0);

    for (section = 1; section < xffEp->sectNrE; section++) {
        if (__inline_checkExistString(xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs, D_00131DD0)) {
            sprintf(
                &buffer, "\t%s\t%p: {*(%s)}\n", xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs,
                xffEp->sectTab[section].memPt, xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs
            );
            LoaderSysFWriteString(linker_fd, (char*)&buffer, 0);
        }
    }

    LoaderSysFWriteString(linker_fd, GSTR(D_00139F48, "}\n"), 1);
    
    LoaderSysFClose(linker_fd);
    return 1; // Sucess
}

extern int D_0013A180;
extern int D_0013A184;
extern char D_00139F50[];
extern char D_0013B4C8[];
void loaderLoop(void);
static inline int _ReadPad() {
    int r;
    r = padSysReadForLoader();
    ExecBaseProc();
    return r;
}

void func_00100A58(void) {
    register void* sp asm("sp");
    register void* gp asm("gp");
    s32 thread_id;
    void* jal_addr;
    s32* call;
    
    thread_id = GetThreadId();
    if (thread_id == D_0013A184) {
        ChangeThreadPriority(GetThreadId(), 1);
        sceSifInitRpc(0);
        sceSifInitIopHeap();
        gp = (void*)&_gp;
        sp = (void*)D_0013A180;
        CancelWakeupThread(D_0013A184);
        WakeupThread(D_0013A184);

        // Take the address of the jal 0 instr
        asm (
            "la %0, __label\n" 
            : "=r"(jal_addr) :
        );

        // Use the uncached address
        call = (int*)((int)jal_addr | 0x20000000);

        // Mask the jal target
        *(call) &= 0xff000000;

        // Set the jal target to loaderLoop
        *(call) |= (((unsigned int)loaderLoop)) / 4;
        
        ___label:
        asm(
            "__label:\n" 
            "jal 0\n"
            "nop\n" 
            : : "r"(gp) // gp use needed so GCC doesn't skip it (why?)
        );
    }

    ChangeThreadPriority(thread_id, 0x7F);
    PutString(0xFF603000, GSTR(D_00139F50, "\n"));
    PutString(0xFF603000, "in threadid: %d\n", thread_id);
    PutString(0xFF603000, D_0013B4C8);
    PutString(0xC0FFE000, "\nhit 'reset' or push 'start key on pad-1' to restart system.");
    PutString(0xC0FFE000, "\n\tor 'source s' to debugging and tracing.");
    LoaderSysPrintf(GSTR(D_00139F50, "\n"));
    LoaderSysSendAbort();
    loaderExecResetCallback();
    
    while(!(_ReadPad() & 0x800)) {
        Sync();
    }
    
    PutString(0xFF603000, GSTR(D_00139F50, "\n"));
    ChangeThreadPriority(GetThreadId(), 0x7F);
    WakeupThread(D_0013A184);
    
    while(1) {
        SleepThread();
    }
}

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

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysGetMemoryInfo);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A00);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A48);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136A80);

INCLUDE_RODATA("asm/nonmatchings/os/loaderSys", D_00136AC0);

extern s32 D_0013A108;        // number of iop modules
extern char D_0013CD10[][16]; // iop module identifiers

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

extern char D_00136A80[]; // "ldsys: setNewIopIdentifier: set new iop identifier \"%s\" at #%d\n", Referenced in LoaderSysLoadIopModuleFromEEBuffer

void setNewIopIdentifier(const char *newIdentifier)
{
    LoaderSysPrintf(D_00136A80, newIdentifier, D_0013A108);
    strncpy(D_0013CD10[D_0013A108++], newIdentifier, strlen(newIdentifier));
}

extern s32 sceSifInitRpc(s32);
extern s32 sceSifInitIopHeap(void);
extern void PutStringS(s32, const char *, ...);
extern s32 sceCdInit(s32);
extern s32 sceCdMmode(s32);
extern s32 sceSifRebootIop(const char *);
extern s32 sceSifSyncIop(void);
extern s32 sceSifLoadFileReset(void);
extern s32 sceFsReset(void);
extern s32 sceDmaReset(s32);
extern s32 PreparePowerOff(void);
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern void setNewIopIdentifier(const char *);
extern s32 sceDbcInit(void);
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
    PutString(0xFFFF00, "\tRebooting Iop\n\t\twith ");
    PutString(0x40FFFF80, GSTR(D_0013A118, "\"%s\""), arg0);
    PutStringS(0xFFFF00, GSTR(D_0013A120, "... "));

    while (sceSifRebootIop(arg0) == 0)
        ;

    while (sceSifSyncIop() == 0)
        ;

    PutStringS(0xFFFF00, GSTR(D_0013A128, "Done.\n"));
}

// TODO: Merge with LoaderSysLoadIopModule once the file is done
static inline s32 __inlined_LoaderSysLoadIopModule(const char *module, s32 arg_count, void *args)
{
    s32 result;

    PutString(0x4080FF00, "\t\tLoading ");
    PutString(0x80C0FF00, GSTR(D_0013A118, "\"%s\""), module);
    PutStringS(0x4080FF00, GSTR(D_0013A120, "... "));
    result = sceSifLoadModule(module, arg_count, args);
    if (result < 0)
    {
        PutStringS(0xFF804000, "ERROR\n\t\t\tCouldn't load \"%s\".\n\t\t\t\t( Error code: %d )\n", module, result);
        return -1;
    }
    PutStringS(0x4080FF00, GSTR(D_0013A128, "Done.\n"));
    return 0;
}

// TODO: Remove once the file is done
static inline void __rodata_LoaderSysUnloadIopModuleByName()
{
    PutStringS(0, "\t\tUnloading ");
    PutStringS(0, "ERROR\n\t\t\tCouldn't unload \"%s\".\n\t\t\t\t( Error code: %d )...\n");
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

    PutString(-256, "Initialize loader (Version: ");
    // PutString(0x80FFC000, GSTR(D_0013A138, "%s %s"), __DATE__, __TIME__);
    PutString(0x80FFC000, GSTR(D_0013A138, "%s %s"), "Jul 12 2005", "16:20:22");
    PutStringS(-256, GSTR(D_0013A140, ")\n\n"));
    PutStringS(0x80C0FF00, "\tWarm up CD/DVD hardware... ");

    sceCdInit(0);
    sceCdMmode(2);

    PutStringS(0x80C0FF00, GSTR(D_0013A128, "Done.\n"));

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
extern s32 D_0013A110;
extern s32 D_0013A184;
extern s32 D_0013A114;
static inline void loaderPrintMessage()
{
    void *heap_base;

    register int sp asm("sp");
    register int gp asm("gp");
    heap_base = (void *)malloc(1);
    free(heap_base);
    PutStringS(0xFFFFFF00, " GP: %08p\n", gp);
    PutStringS(0xFFFFFF00, " SP: %08p\n", sp);
    PutStringS(0xFFFFFF00, " HEAP: %08p\n", heap_base);
    PutStringS(0xFFFFFF00, " END: %08p\n\n", &end);
    PutStringS(0xFFFFFF00, " LOADER HEAP START: %08p\n", D_0013A110);
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
        PutStringS(0xFFFFFF00, "\nreset: %d\n", reset_count);
    }
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", main);

extern const char D_0013D110[]; // Filled at runtime: "cdrom0:\SCPS_15"

const char *LoaderSysGetBootArg(void)
{
    return D_0013D110;
}

s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, void *args)
{
    s32 result;
    PutString(0x4080FF00, "\t\tLoading ");
    PutString(0x80C0FF00, GSTR(D_0013A118, "\"%s\""), path);
    PutStringS(0x4080FF00, GSTR(D_0013A120, "... "));

    result = sceSifLoadModule(path, arg_count, args);

    if (result < 0)
    {
        PutStringS(0xFF804000, "ERROR\n\t\t\tCouldn't load \"%s\".\n\t\t\t\t( Error code: %d )\n", path, result);
        return -1;
    }

    PutStringS(0x4080FF00, GSTR(D_0013A128, "Done.\n"));
    return 0;
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

    PutString(0x4080FF00, "\t\tUnloading ");
    PutString(0x80C0FF00, GSTR(D_0013A118, "\"%s\""), arg0);
    PutStringS(0x4080FF00, GSTR(D_0013A120, "... "));
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
        PutStringS(0xFF804000, "ERROR\n\t\t\tCouldn't unload \"%s\".\n\t\t\t\t( Error code: %d )...\n", arg0, success);
        return -1;
    }

    PutStringS(0x4080FF00, GSTR(D_0013A128, "Done.\n"));
    return success;
}

void LoaderSysHookPoint(void)
{
}

extern s32 sceSifRebootIop(const char *);
extern s32 sceSifSyncIop(void);

void LoaderSysRebootIop(const char *arg0)
{
    PutString(0xFFFF00, "\tRebooting Iop\n\t\twith ");
    PutString(0x40FFFF80, GSTR(D_0013A118, "\"%s\""), arg0);
    PutStringS(0xFFFF00, GSTR(D_0013A120, "... "));

    while (sceSifRebootIop(arg0) == 0)
        ;

    while (sceSifSyncIop() == 0)
        ;

    PutStringS(0xFFFF00, GSTR(D_0013A128, "Done.\n"));
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

void initmemprintf(void)
{
}

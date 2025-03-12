#include "loaderSys.h"
#include "common.h"
#include "fl_xfftype.h"
#include "gcc/string.h"
#include "putString.h"
#include "regnames.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sif.h"
#include "sdk/ee/sifdev.h"

// data
struct unk_00131D00_s D_00131D00 = {
    "noname",
    0x4000,
    0,
};

struct pad_00131D48_s
{
    char padding[0x70];
};
struct pad_00131D48_s D_00131D48 = {""};
IN_DATA char ident[] = "xff2"; // TODO: Maybe figure a natural way to get this into .data?

// sdata vars
s32 D_00139F00 = 0;
void *D_00139F04 = 0; // heap pointer

// sbss vars
extern s32 D_0013A300;

// bss vars
extern char D_0013A4C8[0x1000];

s32 RelocateCode(struct t_xffEntPntHdr *xffEp)
{
    s32 i;
    s32 j;

    while (xffEp != NULL)
    {
        struct t_xffRelocEnt *rt = xffEp->relocTab;
        for (i = xffEp->relocTabNrE; i--; rt++)
        {
            switch (rt->type)
            {
            case 4:
            case 9:
            {
                s32 count = rt->nrEnt;
                for (j = 0; j < count; j++)
                {
                    ResolveRelocation(xffEp, rt, j);
                }
            }
            break;
            }
        }

        if (xffEp->nextXffHdr == 0x0)
        {
            xffEp = NULL;
        }
        else
        {
            printf("ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n", ((u32)xffEp + xffEp->nextXffHdr));
            xffEp = (struct t_xffEntPntHdr *)((u32)xffEp + xffEp->nextXffHdr);
        }
    }

    return 1;
}

s32 FreeDecodedSection(struct t_xffEntPntHdr *xffEp, char (*arg1)(void *))
{
    s32 i;

    while (xffEp != 0x0)
    {
        for (i = 1; i < xffEp->sectNrE; i++)
        {
            struct t_xffSectEnt *st = &xffEp->sectTab[i];
            if (st->moved)
            {
                st->moved = 0;
                arg1(st->memPt);
            }
        }

        if (xffEp->nextXffHdr == 0x0)
        {
            xffEp = 0x0;
        }
        else
        {
            printf("ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n", ((u32)xffEp + xffEp->nextXffHdr));
            xffEp = (struct t_xffEntPntHdr *)((u32)xffEp + xffEp->nextXffHdr);
        }
    }

    return 1;
}

static s32 SearchRelocLoPair(struct t_xffRelocEnt *relocEntry, s32 relocIndex)
{
    s32 i;

    for (i = relocIndex + 1;; i++)
    {
        struct t_xffRelocAddrEnt *addr = &relocEntry->addr[i];
        switch (addr->tyIx & 0xFF)
        {
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
                "ld:\t" ANSI_RED "Warning! Can't find low16 for hi16(relid:%d)." ANSI_RESET "\n", relocIndex);
            return 0;
        }
    }
}

s32 ResolveRelocation(void *xffBuf, struct t_xffRelocEnt *relocEnt, s32 relocIx)
{
    struct t_xffEntPntHdr *xffEp;
    s32 relOffs;
    u32 *relAddr;
    s32 addVal;
    void *tgtAddr;
    s32 loInstr;
    s32 a;
    s32 relTy;
    struct t_xffRelocAddrEnt *addr = &relocEnt->addr[relocIx];

    xffEp = xffBuf;
    relOffs = addr->addr;
    relTy = addr->tyIx & 0xFF;
    relAddr = (void *)((u32)xffEp->sectTab[relocEnt->sect].memPt + relOffs);
    tgtAddr = xffEp->symTab[addr->tyIx >> 8].addr;
    addVal = relocEnt->inst[relocIx].inst;

    switch (relTy)
    {
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
        a >>= 15;
        a++;
        a >>= 1;
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

char *D_00131DC0[] = {"normal use", ANSI_CYAN "out of align(alloc)" ANSI_RESET, "alloc flag(alloc)"};
void DecodeSection(
    void *xffBuf,
    mallocAlign_func *mallocAlign,
    mallocMaxAlign_func *mallocMaxAlign,
    ldrDbgPrintf_func *ldrDbgPrintf)
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

void RelocateElfInfoHeader(struct t_xffEntPntHdr *xffEp)
{
    s32 i;
    u32 xffBaseAddr = (u32)xffEp;

    xffEp->sectTab = (void *)(xffBaseAddr + xffEp->sectTab_Rel);

    for (i = 0; i < xffEp->sectNrE; i++)
    {
        xffEp->sectTab[i].filePt = (void *)(xffBaseAddr + xffEp->sectTab[i].offs_Rel);
    }

    xffEp->symTab = (void *)(xffBaseAddr + xffEp->symTab_Rel);
    xffEp->symRelTab = (void *)(xffBaseAddr + xffEp->symRelTab_Rel);
    xffEp->symTabStr = (void *)(xffBaseAddr + xffEp->symTabStr_Rel);
    xffEp->impSymIxs = (void *)(xffBaseAddr + xffEp->impSymIxs_Rel);
    xffEp->relocTab = (void *)(xffBaseAddr + xffEp->relocTab_Rel);
    xffEp->ssNamesBase = (void *)(xffBaseAddr + xffEp->ssNamesBase_Rel);
    xffEp->ssNamesOffs = (void *)(xffBaseAddr + xffEp->ssNamesOffs_Rel);

    for (i = 0; i < xffEp->relocTabNrE; i++)
    {
        xffEp->relocTab[i].addr = (void *)(xffBaseAddr + xffEp->relocTab[i].addr_Rel);
        xffEp->relocTab[i].inst = (void *)(xffBaseAddr + xffEp->relocTab[i].inst_Rel);
    }

    xffEp->stack = (void *)(xffBaseAddr + xffEp->stack_Rel);
}

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

char *D_00131DD0[12] = {".text", ".vutext", ".data", ".vudata", ".rodata", ".bss", ".vubss", ".DVP.ovlytab"};
u32 D_00139F40 = 0;
static inline void LoaderSysFWriteString(s32 fd, char *str, s32 flush)
{
    s32 len;

    if (D_0013A300 == fd)
    {
        len = strlen(str);

        // Would we go over the buffer with this write? Flush to file
        if ((0xfff < (D_00139F40 + len)) && (D_00139F40 != 0x0))
        {
            LoaderSysFWrite(D_0013A300, &D_0013A4C8, D_00139F40);
            D_00139F40 = 0x0;
        }

        // If the string to write is bigger than the buffer write directly
        // copy to the buffer otherwise
        if (strlen(str) >= 0x1000)
        {
            LoaderSysFWrite(D_0013A300, str, strlen(str));
        }
        else
        {
            strncpy(&D_0013A4C8[D_00139F40], str, strlen(str));
            D_00139F40 += strlen(str);
        }

        // If the flush flag is set then flush everything at the end
        if ((flush) && (D_0013A300 == fd) && (D_00139F40 != 0x0))
        {
            LoaderSysFWrite(D_0013A300, &D_0013A4C8, D_00139F40);
            D_00139F40 = 0x0;
        }
    }
}

s32 OutputLinkerScriptFile(struct t_xffEntPntHdr *xffEp, char *ld_script_path, ldrDbgPrintf_func *callback)
{
    char buffer[1000];
    char pad[16];
    s32 linker_fd;
    s32 symbol;
    s32 section;

    if (callback != NULL)
    {
        callback("ld:\toutput linker script " ANSI_GREEN "\"%s\"" ANSI_RESET ".\n", ld_script_path);
    }

    D_0013A300 = linker_fd = LoaderSysFOpen(ld_script_path, SCE_CREAT | SCE_TRUNC | SCE_WRONLY, 0);

    // Symbol writing might be an inline of its own
    for (symbol = 0; symbol < xffEp->impSymIxsNrE; symbol++)
    {
        sprintf(
            &buffer, "%s = 0x%08x;\n", xffEp->symTabStr + xffEp->symTab[xffEp->impSymIxs[symbol].stIx].nameOffs,
            xffEp->symTab[xffEp->impSymIxs[symbol].stIx].addr);
        LoaderSysFWriteString(linker_fd, (char *)&buffer, 0);
    }

    // Section writing might be an inline of its own
    LoaderSysFWriteString(linker_fd, "SECTIONS {\n", 0);

    for (section = 1; section < xffEp->sectNrE; section++)
    {
        if (_checkExistString(xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs, D_00131DD0))
        {
            sprintf(
                &buffer, "\t%s\t%p: {*(%s)}\n", xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs,
                xffEp->sectTab[section].memPt, xffEp->ssNamesBase + xffEp->ssNamesOffs[section].nmOffs);
            LoaderSysFWriteString(linker_fd, (char *)&buffer, 0);
        }
    }

    LoaderSysFWriteString(linker_fd, "}\n", 1);

    LoaderSysFClose(linker_fd);
    return 1; // Sucess
}

extern s32 D_0013A180;
extern s32 D_0013A184;
extern char D_0013B4C8[];
void loaderLoop(void);
static inline s32 _ReadPad()
{
    s32 r;
    r = padSysReadForLoader();
    ExecBaseProc();
    return r;
}

void func_00100A58(void)
{
    register void *sp asm("sp");
    register void *gp asm("gp");
    s32 thread_id;
    void *jal_addr;
    s32 *call;

    thread_id = GetThreadId();
    if (thread_id == D_0013A184)
    {
        ChangeThreadPriority(GetThreadId(), 1);
        sceSifInitRpc(0);
        sceSifInitIopHeap();
        gp = (void *)&_gp;
        sp = (void *)D_0013A180;
        CancelWakeupThread(D_0013A184);
        WakeupThread(D_0013A184);

        // Take the address of the jal 0 instr
        asm(
            "la %0, __label\n"
            : "=r"(jal_addr) :);

        // Use the uncached address
        call = (s32 *)((s32)jal_addr | 0x20000000);

        // Mask the jal target
        *(call) &= 0xff000000;

        // Set the jal target to loaderLoop
        *(call) |= (((u32)loaderLoop)) / 4;

        asm(
            "__label:\n"
            "jal 0\n"
            "nop\n"
            : : "r"(gp) // gp use needed so GCC doesn't skip it (why?)
        );
    }

    ChangeThreadPriority(thread_id, 0x7F);
    PutString(PUTSTR_COL_ORANGE, "\n");
    PutString(PUTSTR_COL_ORANGE, "in threadid: %d\n", thread_id);
    PutString(PUTSTR_COL_ORANGE, D_0013B4C8);
    PutString(PUTSTR_COL_LLGREEN, "\nhit 'reset' or push 'start key on pad-1' to restart system.");
    PutString(PUTSTR_COL_LLGREEN, "\n\tor 'source s' to debugging and tracing.");
    LoaderSysPrintf("\n");
    LoaderSysSendAbort();
    loaderExecResetCallback();

    while (!(_ReadPad() & 0x800))
    {
        Sync();
    }

    PutString(PUTSTR_COL_ORANGE, "\n");
    ChangeThreadPriority(GetThreadId(), 0x7F);
    WakeupThread(D_0013A184);

    while (1)
    {
        SleepThread();
    }
}

extern char D_0013B4C8[];
void LoaderSysJumpRecoverPointNoStateSetting(char *format, ...)
{
    register void *gp asm("gp");
    va_list args;
    gp = (void *)&_gp;

    LoaderSysExecuteRecoveryFirstProcess();

    va_start(args, format);
    vsprintf(D_0013B4C8, format, args);
    va_end(args);
    func_00100A58();
}

INCLUDE_ASM("asm/nonmatchings/os/loaderSys", LoaderSysJumpRecoverPoint);

void func_00101AA0(void)
{
    register s32 ra asm("ra");
    LoaderSysJumpRecoverPoint("undefined function call from %p.\n", ra - 8);
}

static inline void LoaderSysExecProg(struct t_xffEntPntHdr *xffEp)
{
    s32 (*module_entry)(s32, void *);
    s32 text_nop;

    D_00139F00 = (s32)D_00139F04;
    module_entry = xffEp->entryPnt;
    D_00131D00.unk44 = xffEp;
    text_nop = LoaderSysGetTextNopBase();

    FlushCache(WRITEBACK_DCACHE);
    FlushCache(INVALIDATE_ICACHE);

    LoaderSysPrintf("ld:\texecute entry point(%p) as h10kmode(id:%x)\n", module_entry, text_nop);
    module_entry(1, (void *)text_nop);
}

s32 func_00101B88(struct t_xffEntPntHdr *xffEp, struct t_xffRelocAddrEnt *arg1, unk_stack_40 *arg2)
{
    s32 count;
    s32 i;
    s32 size;
    struct t_xffRelocEnt *srel_ent;
    struct t_xffRelocEnt *trel_ent;
    s8 *data_p;
    struct t_xffEntPntHdr *txffEp;

    txffEp = arg2->unk0;
    trel_ent = arg2->unk8;
    data_p = arg2->unkC;

    count = txffEp->relocTabNrE >> 1;
    srel_ent = &txffEp->relocTab[count];

    for (i = 0; i < count; i++, srel_ent++, trel_ent++)
    {
        size = srel_ent->nrEnt * 8; // sizeof(struct t_xffRelocInstEnt)?
        *trel_ent = *srel_ent;

        memcpy(data_p, srel_ent->addr, size);
        trel_ent->addr = (struct t_xffRelocAddrEnt *)data_p;
        data_p += size;

        memcpy(data_p, srel_ent->inst, size);
        trel_ent->inst = (struct t_xffRelocInstEnt *)data_p;
        data_p += size;
    }

    LoaderSysPrintf("ld:\t\t\tsplit at %p : (duplicate relinfo to %p-%p / reldata to %p-%p)\n", arg1, arg2->unk8, trel_ent, arg2->unkC, data_p);

    arg2->unk4 = count;
    D_00139F04 = arg1;

    return 0;
}

extern s32 D_0013A184;
void LoaderSysResetSystem(void)
{
    s32 thread_id;

    LoaderSysExecuteRecoveryFirstProcess();
    PutString(PUTSTR_COL_ORANGE, "\n");
    thread_id = GetThreadId();
    ChangeThreadPriority(thread_id, 127);
    WakeupThread(D_0013A184);
    do
    {
        SleepThread();
    } while (1);
}

void DisposeRelocationElement(struct t_xffEntPntHdr *xffEp, dispose_reloc_func *arg1, void *arg2)
{
    s32 i;
    s32 reloc_count;
    s32 total_relocs;

    if (xffEp->ident == *(u32 *)ident)
    {
        reloc_count = xffEp->relocTabNrE >> 1;

        total_relocs = 0x0;
        for (i = 0; i < reloc_count; i++)
        {
            total_relocs += xffEp->relocTab[i].nrEnt;
        }

        arg1(xffEp, &xffEp->relocTab->addr[total_relocs], arg2);

        for (i = 0; i < reloc_count; i++)
        {
            xffEp->relocTab[reloc_count + i].nrEnt = 0;
        }
    }
}

s32 RelocateSelfSymbol(struct t_xffEntPntHdr *xffEp, void *arg1)
{
    s32 symtab_count = xffEp->symTabNrE;
    struct t_xffSymEnt *st = &xffEp->symTab[0];
    struct t_xffSymRelEnt *rt = &xffEp->symRelTab[0];
    struct t_xffSectEnt *stt = &xffEp->sectTab[0];

    for (; symtab_count--; st++, rt++)
    {
        u32 section = st->sect;

        switch (section)
        {
        case 0:
            st->addr = arg1;
            break;
        case 0xFFF1:
            st->addr = (void *)rt->offs;
            break;
        default:
            if (section <= 0xFEFF)
            {
                switch (st->type & 0xF)
                {
                case 0:
                case 1:
                case 2:
                case 3:
                    st->addr = (void *)(rt->offs + (s32)stt[section].memPt);
                    break;
                }
            }
            break;
        }
    }

    return 1;
}

s32 LoaderSysRelocateOnlineElfInfo(struct t_xffEntPntHdr *xffEp, void *arg1, void *arg2, void *arg3, void *arg4)
{
    RelocateElfInfoHeader(xffEp);
    DecodeSection(xffEp, arg1, arg2, arg4);
    RelocateSelfSymbol(xffEp, arg3);
    RelocateCode(xffEp);

    return 1;
}

void *mallocAlignMempool(s32 size, u32 align)
{
    void *result = (void *)((((u32)D_00139F04 + align - 1) / align) * align);
    D_00139F04 = (void *)((u32)result + size);
    return result;
}

void *mallocAlign0x100Mempool(s32 size)
{
    return mallocAlignMempool(size, 0x100);
}

struct t_xffEntPntHdr *func_00100D48(char *module_path)
{
    s32 fid;
    u8 *chr_p;
    struct sce_stat fstat;
    unk_stack_40 sp40;
    char linkfile_path[0x400];
    s32 i;
    s32 j;
    s32 rcount;
    struct t_xffEntPntHdr *xffEp;
    struct t_xffRelocEnt *rt;
    u64 size;
    s32 chksum;
    typedef void(vLoaderSysPrintf)(char *, ...);

    LoaderSysPrintf("ld:\tload module \"%s\"\n", module_path);

    fid = LoaderSysGetstat(module_path, &fstat);
    if (fid != 0x0)
    {
        PutString(PUTSTR_COL_WHITE, "Opening file \"%s\" failed(0x%x).\n", module_path, fid);
        return NULL;
    }

    fid = LoaderSysFOpen(module_path, 0x1, 0x0);
    if (fid < 0x0)
    {
        PutString(PUTSTR_COL_WHITE, "Opening file \"%s\" failed(0x%x).\n", module_path, fid);
        return NULL;
    }

    size = ((s64)fstat.st_hisize << 32) | fstat.st_size;
    xffEp = (void *)mallocAlignMempool(size, 0x40);
    LoaderSysFRead(fid, xffEp, (u32)size);
    LoaderSysFClose(fid);

    chksum = 0x0;
    chr_p = (u8 *)xffEp;
    for (i = size; i--;)
    {
        chksum += *chr_p++;
    }

    // Is this (somehow) an inline? This call needs to return void
    ((vLoaderSysPrintf *)&LoaderSysPrintf)("ld:\t\t0x%08x - 0x%08x (0x%08x:%x)\n", xffEp, ((s32)xffEp + (s32)size), size, chksum);

    sp40.unk8 = (struct t_xffRelocEnt *)0x1000000;
    sp40.unkC = (void *)0x1100000;
    sp40.unk0 = xffEp;
    sp40.unk4 = 0;

    RelocateElfInfoHeader(xffEp);
    DisposeRelocationElement(xffEp, &func_00101B88, &sp40);
    DecodeSection(xffEp, &mallocAlignMempool, &mallocAlign0x100Mempool, &LoaderSysPrintf);
    RelocateSelfSymbol(xffEp, &func_00101AA0);
    RelocateCode(xffEp);

    // inline?
    rt = &sp40.unk8[0];
    for (rcount = sp40.unk4; rcount--; rt++)
    {
        switch (rt->type)
        {
        case 4:
        case 9:
        {
            s32 count = rt->nrEnt;
            for (j = 0; j < count; j++)
            {
                ResolveRelocation(xffEp, rt, j);
            }
        }
        break;
        }
    }

    sprintf(linkfile_path, "%s.cmd", module_path);
    OutputLinkerScriptFile(xffEp, linkfile_path, &LoaderSysPrintf);
    return xffEp;
}

void *MoveElf(struct t_xffEntPntHdr *xffEp, void *arg1)
{
    LoaderSysPrintf("%p: %x => %p\n", xffEp, xffEp->stack_Rel, arg1);
    xffEp = memmove(arg1, xffEp, xffEp->stack_Rel);
    FlushCache(0);
    LoaderSysRelocateOnlineElfInfo(xffEp, &mallocAlignMempool, &mallocAlign0x100Mempool, &func_00101AA0, &LoaderSysPrintf);
    FlushCache(0);
    LoaderSysPrintf("entry: %p\n", xffEp->entryPnt);
    return xffEp;
}

// TODO: Use headers
extern s32 LoaderSysFRead(s32 fd, void *buf, s32 count);
unk_00131D00_s *func_001013C8(const char *filename)
{
    struct sce_stat stat;
    s32 th_id;
    u64 size;
    s32 i;
    char *p;

    D_00131D00.stack_size = 0x4000;

    th_id = LoaderSysGetstat(filename, &stat);
    if (th_id != 0)
    {
        PutStringS(PUTSTR_COL_LORANGE, "Can't execute,\nbecause start file:\"%s\" can't read(%d).\n", filename, th_id);
        return 0;
    }

    th_id = LoaderSysFOpen(filename, 1, 0);
    if (th_id < 0)
    {
        D_00131D00.path[0] = '\0';
        PutStringS(PUTSTR_COL_LORANGE, "Can't execute,\nbecause start file:\"%s\" can't read(%d).\n", filename, th_id);
        return 0;
    }

    size = (u64)stat.st_hisize << 32 | (u64)stat.st_size;

    LoaderSysFRead(th_id, D_00131D00.path, size);
    LoaderSysFClose(th_id);

    D_00131D00.path[size] = '\0';
    D_00131D00.path[64 - 1] = '\0';

    p = D_00131D00.path;
    for (i = 0; i < size; i++)
    {
        if (p[i] == ' ')
        {
            p[i] = '\0';
            sscanf(&p[i + 1], "0x%x", &D_00131D00.stack_size);
            break;
        }
    }

    return &D_00131D00;
}

void SetHeapStartPoint(u32 start_address)
{
    HEAP_START = (void *)((start_address + 0xF) & ~0xF);
}

s32 GetHeapCurrentPoint(void)
{
    return (u32)HEAP_START;
}

void _execProgWithThread(void *module_path)
{
    s32 (*module_entry)(s32, void *);
    s32 text_nop;
    s32 th_id;
    struct t_xffEntPntHdr *xffEp;

    th_id = GetThreadId();
    xffEp = func_00100D48((char *)module_path);

    if (xffEp != NULL)
    {
        D_00139F00 = (s32)D_00139F04;
        module_entry = xffEp->entryPnt;
        D_00131D00.unk44 = xffEp;
        text_nop = LoaderSysGetTextNopBase();

        FlushCache(WRITEBACK_DCACHE);
        FlushCache(INVALIDATE_ICACHE);

        LoaderSysPrintf("ld:\texecute entry point(%p) as h10kmode(id:%x)\n", module_entry, text_nop);
        module_entry(1, (void *)text_nop);
    }
    else
    {
        PutStringS(PUTSTR_COL_LORANGE, "Can't execute, because map fail.\n");
    }
    LoaderSysDeleteExternalThreadList(th_id);
    ExitDeleteThread();
}

s32 execProgWithThread(const char *filename, s32 priority)
{
    s32 th_id;
    unk_00131D00_s *prog_info;

    prog_info = func_001013C8(filename);
    if (prog_info != NULL)
    {
        struct ThreadParam prog_thread = {
            .entry = _execProgWithThread,
            .stack = mallocAlignMempool(prog_info->stack_size, 0x10),
            .gpReg = &_gp,
            .initPriority = priority,
            .stackSize = prog_info->stack_size};

        th_id = CreateThread(&prog_thread);
        LoaderSysEntryExternalThreadList(th_id);
        StartThread(th_id, prog_info);
        return (s32)prog_thread.stack;
    }
}

#ifndef LOADERSYS_H
#define LOADERSYS_H

#include "include_asm.h"
#include "common.h"
#include "fl_xfftype.h"
#include "sdk/ee/eekernel.h"
#include "gcc/string.h"

#define R_MIPS_NONE (0)
#define R_MIPS_16 (1)
#define R_MIPS_32 (2)
#define R_MIPS_REL32 (3)
#define R_MIPS_26 (4)
#define R_MIPS_HI16 (5)
#define R_MIPS_LO16 (6)

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

typedef int (dispose_reloc_func)(struct t_xffEntPntHdr*, struct t_xffRelocAddrEnt*, unk_stack_40*);
typedef void*(mallocAlign_func)(s32, u32);
typedef void*(mallocMaxAlign_func)(s32);
typedef int (ldrDbgPrintf_func)(const char *, ...);

s32 ResolveRelocation(void* xffBuf, struct t_xffRelocEnt* relocEnt, s32 relocIx);
void DecodeSection(void *, mallocAlign_func*, mallocMaxAlign_func*, ldrDbgPrintf_func*);
void RelocateElfInfoHeader(struct t_xffEntPntHdr* xffEp);
s32 OutputLinkerScriptFile(struct t_xffEntPntHdr*, char*, ldrDbgPrintf_func*);
void func_00100A58(void);
void LoaderSysJumpRecoverPointNoStateSetting(char* format, ...);
void* LoaderSysJumpRecoverPoint(char* format, ...);
struct t_xffEntPntHdr* func_00100D48(char* module_path);
void* MoveElf(struct t_xffEntPntHdr* xffEp, void* arg1);
unk_00131D00_s* func_001013C8(const char* filename);
s32 LoaderSysRelocateOnlineElfInfo(struct t_xffEntPntHdr* xffEp, void* arg1, void* arg2, void* arg3, void* arg4);
s32 RelocateCode(struct t_xffEntPntHdr* xffEp);
s32 FreeDecodedSection(struct t_xffEntPntHdr* xffEp, char (*arg1)(void*));
s32 RelocateSelfSymbol(struct t_xffEntPntHdr* xffEp, void* arg1);
void DisposeRelocationElement(struct t_xffEntPntHdr* xffEp, dispose_reloc_func* arg1, void* arg2);
void SetHeapStartPoint(u32 start_address);
s32 GetHeapCurrentPoint(void);
void LoaderSysResetSystem(void);
void func_00101AA0(void);
void* mallocAlignMempool(s32, u32);
void* mallocAlign0x100Mempool(s32);
s32 _checkExistString(char *string, char **strings);
s32 func_00101B88(struct t_xffEntPntHdr* xffEp, struct t_xffRelocAddrEnt *arg1, unk_stack_40* arg2);
void _execProgWithThread(void* module_path);
s32 execProgWithThread(const char* filename, s32 priority);

extern s32 LoaderSysPrintf(const char *format, ...);

// Legitimate Static inlines
// TODO: This points to a custom section, so this should be a linker symbol
static inline int LoaderSysGetTextNopBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(func_00131CAC)\n" "nop\n"
        "addiu %0,%%lo(func_00131CAC)\n" "nop\n"
         : "=r"(ret) :
    );
    return ret;
}

#endif /* LOADERSYS_H */

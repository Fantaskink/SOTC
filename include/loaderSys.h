#ifndef LOADERSYS_H
#define LOADERSYS_H

#include "common.h"
#include "fl_xfftype.h"
#include "gcc/string.h"
#include "include_asm.h"
#include "sdk/ee/eekernel.h"

#define R_MIPS_NONE (0)
#define R_MIPS_16 (1)
#define R_MIPS_32 (2)
#define R_MIPS_REL32 (3)
#define R_MIPS_26 (4)
#define R_MIPS_HI16 (5)
#define R_MIPS_LO16 (6)

extern void *D_00139F04; // heap pointer
#define HEAP_START D_00139F04
#define MAX_RESET_CALLBACKS 10

typedef struct unk_stack_40
{
    struct t_xffEntPntHdr *unk0;
    s32 unk4;
    struct t_xffRelocEnt *unk8;
    void *unkC;
} unk_stack_40;

typedef struct unk_00131D00_s
{
    char path[0x40];
    s32 stack_size;
    struct t_xffEntPntHdr *unk44;
} unk_00131D00_s;

typedef s32(dispose_reloc_func)(struct t_xffEntPntHdr *, struct t_xffRelocAddrEnt *, unk_stack_40 *);
typedef void *(mallocAlign_func)(s32, u32);
typedef void *(mallocMaxAlign_func)(s32);
typedef s32(ldrDbgPrintf_func)(const char *, ...);

s32 ResolveRelocation(void *xffBuf, struct t_xffRelocEnt *relocEnt, s32 relocIx);
void DecodeSection(void *, mallocAlign_func *, mallocMaxAlign_func *, ldrDbgPrintf_func *);
void RelocateElfInfoHeader(struct t_xffEntPntHdr *xffEp);
s32 OutputLinkerScriptFile(struct t_xffEntPntHdr *, char *, ldrDbgPrintf_func *);
void func_00100A58(void);
void LoaderSysJumpRecoverPointNoStateSetting(char *format, ...);
void *LoaderSysJumpRecoverPoint(char *format, ...);
struct t_xffEntPntHdr *func_00100D48(char *module_path);
void *MoveElf(struct t_xffEntPntHdr *xffEp, void *arg1);
unk_00131D00_s *func_001013C8(const char *filename);
s32 LoaderSysRelocateOnlineElfInfo(struct t_xffEntPntHdr *xffEp, void *arg1, void *arg2, void *arg3, void *arg4);
s32 RelocateCode(struct t_xffEntPntHdr *xffEp);
s32 FreeDecodedSection(struct t_xffEntPntHdr *xffEp, char (*arg1)(void *));
s32 RelocateSelfSymbol(struct t_xffEntPntHdr *xffEp, void *arg1);
void DisposeRelocationElement(struct t_xffEntPntHdr *xffEp, dispose_reloc_func *arg1, void *arg2);
void SetHeapStartPoint(u32 start_address);
s32 GetHeapCurrentPoint(void);
void LoaderSysResetSystem(void);
void func_00101AA0(void);
void *mallocAlignMempool(s32, u32);
void *mallocAlign0x100Mempool(s32);
s32 _checkExistString(char *string, char **strings);
s32 func_00101B88(struct t_xffEntPntHdr *xffEp, struct t_xffRelocAddrEnt *arg1, unk_stack_40 *arg2);
void _execProgWithThread(void *module_path);
s32 execProgWithThread(const char *filename, s32 priority);


// Legitimate Static inlines
// TODO: This points to a custom section, so this should be a linker symbol
static inline s32 LoaderSysGetTextNopBase()
{
    s32 ret;
    asm volatile(
        "lui   %0,%%hi(func_00131CAC)\n"
        "nop\n"
        "addiu %0,%%lo(func_00131CAC)\n"
        "nop\n"
        : "=r"(ret) :);
    return ret;
}

#endif /* LOADERSYS_H */

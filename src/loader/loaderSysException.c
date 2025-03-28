#include "loaderSysException.h"
#include "common.h"
#include "exceptData.h"
#include "loaderSys.h"
#include "loaderSys3.h"
#include "loaderSysFileIO.h"
#include "putString.h"
#include "regnames.h"
#include "termcolor.h"

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

extern const char D_0013A0F0[];
qword D_0013B900 __attribute__((aligned(64)));

static inline void LoaderSysDumpRegisterList(unk_except_s *ctx)
{
    s32 i;
    for (i = 0; i < 8; i++)
    {
        PutString(PUTSTR_COL_LLBLUE2, "     %s:%08x %s:%08x %s:%08x %s:%08x \n",
                  ctx[(i * 4) + 0].name, ctx[(i * 4) + 0].value,
                  ctx[(i * 4) + 1].name, ctx[(i * 4) + 1].value,
                  ctx[(i * 4) + 2].name, ctx[(i * 4) + 2].value,
                  ctx[(i * 4) + 3].name, ctx[(i * 4) + 3].value);
    }
}

void func_00101EC0(s32 except_code, u32 cause, u32 epc, u32 bva, u32 bpa, unk_except_s *ctx, u32 mode)
{
    s32 i;

    switch (mode)
    {
    case 0:
        PutString(PUTSTR_COL_YELLOW, "      %s\n\n", D_00131E00[except_code].name);
        PutString(PUTSTR_COL_TEAL, "   pc:%08x ", epc);
        PutString(PUTSTR_COL_LBLUE2, "bva:%08x bpa:%08x ", bva, bpa);
        PutString(PUTSTR_COL_GRAY, "cause:%08x\n", cause);
        PutString(PUTSTR_COL_WHITE, "\n");
        LoaderSysDumpRegisterList(ctx);
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

    PutString(PUTSTR_COL_WHITE, "\n");
    PutString(PUTSTR_COL_LORANGE2, "     -----------------------------------------------\n");
    PutString(PUTSTR_COL_LORANGE2, "           ios reports critical error message.\n");
    PutString(PUTSTR_COL_LORANGE2, "     -----------------------------------------------\n");
    PutString(PUTSTR_COL_ORANGE, "\n");
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

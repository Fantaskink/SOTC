#include "putString.h"
#include "common.h"

extern s32 D_0013A26C;
extern s32 D_0013A270;
extern struct t_PutStringFBChar D_0013EF10[PUT_STRING_FB_HGHT][PUT_STRING_FB_WDTH];

INCLUDE_ASM("asm/nonmatchings/os/putString", PutFont);

INCLUDE_ASM("asm/nonmatchings/os/putString", _putString);

INCLUDE_ASM("asm/nonmatchings/os/putString", PutString);

INCLUDE_ASM("asm/nonmatchings/os/putString", PutStringS);

INCLUDE_ASM("asm/nonmatchings/os/putString", func_00105A60);

INCLUDE_ASM("asm/nonmatchings/os/putString", func_00105C50);

INCLUDE_ASM("asm/nonmatchings/os/putString", Sync);

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137540);

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137D40);

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137D50);

INCLUDE_ASM("asm/nonmatchings/os/putString", ReinitDisp);

INCLUDE_ASM("asm/nonmatchings/os/putString", LoaderSysDrawSprite);

INCLUDE_ASM("asm/nonmatchings/os/putString", ExecBaseProc);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetLocate);

INCLUDE_ASM("asm/nonmatchings/os/putString", ScrollDisplay);

INCLUDE_ASM("asm/nonmatchings/os/putString", ClearDisplay);

INCLUDE_ASM("asm/nonmatchings/os/putString", InitDisp);

INCLUDE_ASM("asm/nonmatchings/os/putString", RestoreNormalDrawEnvironment);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetTextureWithFrameBuffer);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetTexDrawEnvironment);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetDrawnTextureEnvironment);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetDrawEnvironment);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetPrimColor);

INCLUDE_ASM("asm/nonmatchings/os/putString", SetPrimColorTex);

void PutChar(PutStringColor color, s8 ch)
{
    D_0013EF10[D_0013A270][D_0013A26C].color = color;
    D_0013EF10[D_0013A270][D_0013A26C].ch = ch;
}

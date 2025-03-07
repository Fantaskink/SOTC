#include "putString.h"
#include "common.h"
#include "sdk/ee/eestruct.h"
#include "sdk/ee/eetypes.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

extern u128 D_00137D50;
extern u32 D_0013A244;
extern u32 D_0013A24C;
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

void SetPrimColorTex(s32 prim_type, s32 r, s32 g, s32 b, s32 a, s32 use_uv)
{
    sceVif1Packet pkt;
    sceDmaChan *dma_chan;

    sceVif1PkInit(&pkt, (u128 *)(0x70000000 | (D_0013A244 << 0xd)));
    sceVif1PkReset(&pkt);
    sceVif1PkCnt(&pkt, 0);
    sceVif1PkOpenDirectCode(&pkt, 0);

    D_0013A244 = (D_0013A244 + 1) & 1;

    sceVif1PkOpenGifTag(&pkt, D_00137D50);
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_PRIM(prim_type, 0, 1, 0, 0, 0, use_uv != 0, 0, 0));
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_RGBAQ(r, g, b, a, D_0013A24C));
    sceVif1PkCloseGifTag(&pkt);
    sceVif1PkCloseDirectCode(&pkt);
    sceVif1PkEnd(&pkt, 0);
    sceVif1PkTerminate(&pkt);

    sceGsSyncPath(0, 0);

    dma_chan = sceDmaGetChan(1);
    dma_chan->chcr.TTE = 1;
    sceDmaSend(dma_chan, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

void PutChar(PutStringColor color, char ch)
{
    D_0013EF10[D_0013A270][D_0013A26C].color = color;
    D_0013EF10[D_0013A270][D_0013A26C].ch = ch;
}

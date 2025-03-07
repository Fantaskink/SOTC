#include "putString.h"
#include "common.h"
#include "sdk/ee/eetypes.h"
#include "sdk/ee/eestruct.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

// From samples
typedef struct
{
    sceGifTag giftag;
    sceGsTest gs_test;
    s64 gs_test1addr;
    sceGsAlphaEnv gs_alpha;
    sceGsTexEnv gs_tex;
} TexEnv;

typedef struct
{
    sceGifTag giftag;
    sceGsDrawEnv1 draw;
} DrawEnv;

extern TexEnv D_001320D0;
extern DrawEnv D_00132170;
extern TexEnv D_00132200;
extern TexEnv D_001322A0;

extern u128 D_00137D40;
extern u128 D_00137D50;
extern u32 D_0013A244;
extern u32 D_0013A248;
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

void SetDrawEnvironment(s32 mode)
{
    TexEnv *tx_env;

    tx_env = (TexEnv *)((s32)&D_001322A0 | 0x20000000);
    sceGsSetDefAlphaEnv(&tx_env->gs_alpha, 0);
    switch (mode)
    {
    case 0:
        *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_ZERO, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
        break;
    case 1:
        *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_CD, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
        break;
    case 2:
        *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_ZERO, SCE_GS_ALPHA_CS, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
        break;
    case 3:
        *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_CD, SCE_GS_ALPHA_FIX, SCE_GS_ALPHA_CD, 0x28);
        break;
    case 4:
        *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_ZERO, SCE_GS_ALPHA_FIX, SCE_GS_ALPHA_CD, 0);
        break;
    }

    *(u64 *)&tx_env->gs_test = SCE_GS_SET_TEST(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_FALSE, SCE_GS_ALPHA_ALWAYS);
    tx_env->gs_test1addr = SCE_GS_TEST_1;
    sceGsSyncPath(0, 0);
    sceGsPutDrawEnv(&tx_env->giftag);
}

void SetPrimColor(s32 prim_type, s32 r, s32 g, s32 b, s32 a)
{
    sceVif1Packet pkt;
    sceDmaChan *dmaVif;

    sceVif1PkInit(&pkt, (u128 *)(0x70000000 | (D_0013A244 << 0xd)));
    sceVif1PkReset(&pkt);
    sceVif1PkCnt(&pkt, 0);
    sceVif1PkOpenDirectCode(&pkt, 0);

    D_0013A244 = (D_0013A244 + 1) & 1;

    sceVif1PkOpenGifTag(&pkt, D_00137D40);
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_PRIM(prim_type, 0, 0, 0, 1, 0, 0, 0, 0));
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_RGBAQ(r, g, b, a, D_0013A248));
    sceVif1PkCloseGifTag(&pkt);
    sceVif1PkCloseDirectCode(&pkt);
    sceVif1PkEnd(&pkt, 0);
    sceVif1PkTerminate(&pkt);

    sceGsSyncPath(0, 0);

    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;
    sceDmaSend(dmaVif, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

void SetPrimColorTex(s32 prim_type, s32 r, s32 g, s32 b, s32 a, s32 use_uv)
{
    sceVif1Packet pkt;
    sceDmaChan *dmaVif;

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

    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;
    sceDmaSend(dmaVif, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

void PutChar(PutStringColor color, char ch)
{
    D_0013EF10[D_0013A270][D_0013A26C].color = color;
    D_0013EF10[D_0013A270][D_0013A26C].ch = ch;
}

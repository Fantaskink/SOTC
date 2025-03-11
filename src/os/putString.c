#include "putString.h"
#include "common.h"
#include "sdk/ee/eestruct.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

INCLUDE_ASM("asm/nonmatchings/os/putString", PutFont);

INCLUDE_ASM("asm/nonmatchings/os/putString", _putString);

void PutString(PutStringColor color, const char *format, ...)
{
    char buffer[0x100];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    _putString(color, buffer);
    va_end(args);

    if (D_0013A280 >= 4)
    {
        Sync();
        ExecBaseProc();
        D_0013A280 = 0;
    }
    else
    {
        D_0013A280++;
    }
}

void PutStringS(PutStringColor color, const char *format, ...)
{
    char buffer[0x100];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    _putString(color, buffer);

    Sync();
    ExecBaseProc();

    va_end(args);
}

INCLUDE_ASM("asm/nonmatchings/os/putString", func_00105A60);

void func_00105C50(void) {
    s32 x;

    if ((D_0013A274 >> 4) & 1) {
        __inlined_PutChar(0xFFFFFF00, 0x20);
    } else {
        __inlined_PutChar(0xFFFFFF00, 0x12);
    }
    D_0013A274++;
    D_0013A274 = D_0013A274 & 0xFF;
}

void Sync(void) {
    FlushCache(0);
    sceGsSyncPath(1, 0);
    D_0013A240 = sceGsSyncV(0);
    D_0013A238 = (D_0013A23C+1) & 1;
    D_0013A23C++;
    sceGsSwapDBuff(&D_0013ECE0, D_0013A238);
    __inlined_RestoreNormalDrawEnvironment(&D_0013ECE0, D_0013A238, D_0013A240);
    __inlined_SetDrawEnvironment(1);
}

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137540);

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137D40);

INCLUDE_RODATA("asm/nonmatchings/os/putString", D_00137D50);

void ReinitDisp(void)
{
    sceGsDBuff *buff = &D_0013ECE0;
    sceGsSyncV(0);
    sceDmaReset(1);
    sceDmaSync(sceDmaGetChan(SCE_DMA_VIF0), 0, 0x64);
    sceDmaSync(sceDmaGetChan(SCE_DMA_VIF1), 0, 0x64);
    sceDmaSync(sceDmaGetChan(SCE_DMA_GIF), 0, 0x64);
    sceDmaGetChan(SCE_DMA_VIF0)->chcr.TTE = 1;
    sceDmaGetChan(SCE_DMA_VIF1)->chcr.TTE = 1;
    sceDmaGetChan(SCE_DMA_GIF)->chcr.TTE = 1;
    sceGsResetPath();
    sceGsSyncV(0);
    D_0013A230 = 640;
    D_0013A234 = 448;
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FIELD);
    sceGsSetDefDBuff(buff, SCE_GS_PSMCT32, D_0013A230, D_0013A234, SCE_GS_DEPTH_GEQUAL, SCE_GS_PSMCT32, SCE_GS_CLEAR);
    sceGsSyncV(0);
    D_0013A238 = 0;
    FlushCache(WRITEBACK_DCACHE);
    sceGsSwapDBuff(buff, D_0013A238);

    while (sceGsSyncPath(1, 0) != 0)
    {
        scePrintf("wait gs init\n");
    }
}

void LoaderSysDrawSprite(xypair* arg0, uvpair* arg1, rgba32* color, s32 arg3, s32 arg4) {
    sceVif1Packet sp;
    sceDmaChan *dmaVif2;
    sceDmaChan *dmaVif3;

    if (arg4 != 0) {
        __inlined_SetPrimColorTex(SCE_GS_PRIM_SPRITE, color->r, color->g, color->b, color->a, 1);

        sceVif1PkInit(&sp, (u128 *)((D_0013A244 << 0xD) | 0x70000000));
        sceVif1PkReset(&sp);
        sceVif1PkCnt(&sp, 0);
        sceVif1PkOpenDirectCode(&sp, 0);
        D_0013A244 = (D_0013A244 + 1) & 1;
        sceVif1PkOpenGifTag(&sp, D_00132350);
        sceVif1PkAddGsData(&sp, SCE_GS_SET_UV(arg1->u0, arg1->v0));
        sceVif1PkAddGsData(&sp, SCE_GS_SET_XYZF2((arg0->x0 + 0x800) << 4, (arg0->y0 + 0x800) << 4, 0x800000, 0));
        sceVif1PkAddGsData(&sp, SCE_GS_SET_UV(arg1->u1 << 4, arg1->v1 << 4));
        sceVif1PkAddGsData(&sp, SCE_GS_SET_XYZF2((arg0->x1 + 0x800) << 4, (arg0->y1 + 0x800) << 4, 0x800000, 0));
        sceVif1PkCloseGifTag(&sp);
        sceVif1PkCloseDirectCode(&sp);
        sceVif1PkEnd(&sp, 0);
        sceVif1PkTerminate(&sp);
        sceGsSyncPath(0, 0);
        dmaVif2 = sceDmaGetChan(1);
        dmaVif2->chcr.TTE = 1;
        sceDmaSend(dmaVif2, (u128*)(0x80000000 | ((int)sp.pBase & 0x3FF0)));
    } else {
        __inlined_SetPrimColorTex(SCE_GS_PRIM_SPRITE, color->r, color->g, color->b, color->a, 1);
        
        sceVif1PkInit(&sp, (u128 *)((D_0013A244 << 0xD) | 0x70000000));
        sceVif1PkReset(&sp);
        sceVif1PkCnt(&sp, 0);
        sceVif1PkOpenDirectCode(&sp, 0);
        D_0013A244 = (D_0013A244 + 1) & 1;
        sceVif1PkOpenGifTag(&sp, D_00132360);
        sceVif1PkAddGsData(&sp, SCE_GS_SET_XYZF2((arg0->x0 + 0x800) << 4, (arg0->y0 + 0x800) << 4, 0x800000, 0));
        sceVif1PkAddGsData(&sp, SCE_GS_SET_XYZF2((arg0->x1 + 0x800) << 4, (arg0->y1 + 0x800) << 4, 0x800000, 0));
        sceVif1PkCloseGifTag(&sp);
        sceVif1PkCloseDirectCode(&sp);
        sceVif1PkEnd(&sp, 0);
        sceVif1PkTerminate(&sp);
        sceGsSyncPath(0, 0);
        dmaVif3 = sceDmaGetChan(1);
        dmaVif3->chcr.TTE = 1;
        sceDmaSend(dmaVif3, (u128*)(0x80000000 | ((int)sp.pBase & 0x3FF0)));
    }
}

void ExecBaseProc(void)
{
    TexEnv *tx_env;

    tx_env = (TexEnv *)((s32)&D_001322A0 | 0x20000000);
    sceGsSetDefAlphaEnv(&tx_env->gs_alpha, 0);
    *(u64 *)&tx_env->gs_alpha.alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_ZERO, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
    *(u64 *)&tx_env->gs_test = SCE_GS_SET_TEST(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_FALSE, SCE_GS_ALPHA_ALWAYS);
    tx_env->gs_test1addr = SCE_GS_TEST_1;
    sceGsSyncPath(0, 0);
    sceGsPutDrawEnv(&tx_env->giftag);
    func_00105C50();
    func_00105A60();
}

void SetLocate(s32 x, s32 y)
{
    D_0013A26C = x;
    D_0013A270 = y;
}

void ScrollDisplay(void)
{
    s32 i, j;

    for (i = 1; i < PUT_STRING_FB_HGHT; i++)
    {
        for (j = 0; j < PUT_STRING_FB_WDTH; j++)
        {
            D_0013EF10[i - 1][j] = D_0013EF10[i][j];
        }
    }

    for (j = 0; j < PUT_STRING_FB_WDTH; j++)
    {
        D_0013EF10[PUT_STRING_FB_HGHT - 1][j] = D_0013A260;
    }
}

void ClearDisplay(void)
{
    s32 i, j;

    for (i = 0; i < PUT_STRING_FB_HGHT; i++)
    {
        for (j = 0; j < PUT_STRING_FB_WDTH; j++)
        {
            D_0013EF10[i][j] = D_0013A260;
        }
    }
    return;
}

void InitDisp(void)
{
    ReinitDisp();
    __inlined_ClearDisplay();
    __inlined_SetLocate(0, 0);
}

void RestoreNormalDrawEnvironment(sceGsDBuff *dbuff, s32 arg1, s32 half_off)
{
    sceGsDBuff *udbuf;

    udbuf = (sceGsDBuff *)((s32)dbuff | 0x20000000);
    if (arg1 != 0)
    {
        sceGsSetHalfOffset(&udbuf->draw1, 0x800, 0x800, half_off);
        sceGsPutDrawEnv(&udbuf->giftag1);
    }
    else
    {
        sceGsSetHalfOffset(&udbuf->draw0, 0x800, 0x800, half_off);
        sceGsPutDrawEnv(&udbuf->giftag0);
    }
}

void SetTextureWithFrameBuffer(s16 tbp0)
{
    sceGsAlphaEnv *alpha;
    TexEnv *tx_env;

    tx_env = (TexEnv *)((s32)&D_001320D0 | 0x20000000);
    alpha = &tx_env->gs_alpha;
    sceGsSetDefTexEnv(&tx_env->gs_tex, 0, tbp0, D_0013A230 / 0x40, SCE_GS_PSMCT24, 10, 8, 0, 0, 0, 0, 1);
    sceGsSetDefAlphaEnv(alpha, 0);
    *(u64 *)&alpha->alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_CD, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
    *(u64 *)&tx_env->gs_test = SCE_GS_SET_TEST(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_FALSE, SCE_GS_ALPHA_ALWAYS);
    tx_env->gs_test1addr = SCE_GS_TEST_1;
    *(u64 *)&tx_env->gs_tex.tex11 = SCE_GS_SET_TEX1(1, 0, SCE_GS_LINEAR, SCE_GS_LINEAR, 0, 0, 0);
    *(u64 *)&tx_env->gs_tex.clamp1 = SCE_GS_SET_CLAMP(SCE_GS_REPEAT, SCE_GS_REPEAT, 0, 0, 0, 0);
    sceGsSyncPath(0, 0);
    sceGsPutDrawEnv(&tx_env->giftag);
}

void SetTexDrawEnvironment(s32 arg0)
{
    DrawEnv *dw_env;

    dw_env = (DrawEnv *)((s32)&D_00132170 | 0x20000000);
    sceGsSetDefDrawEnv(&dw_env->draw, SCE_GS_PSMCT32, D_0013A230, D_0013A234, 1, SCE_GS_PSMZ24);
    dw_env->draw.frame1.FBP = arg0 / 0x20;
    dw_env->draw.zbuf1.ZMSK = 1;
    sceGsSyncPath(0, 0);
    sceGsPutDrawEnv(&dw_env->giftag);
}

void SetDrawnTextureEnvironment(s16 tbp0)
{
    sceGsAlphaEnv *alpha;
    TexEnv *tx_env;

    tx_env = (TexEnv *)((s32)&D_00132200 | 0x20000000);
    alpha = &tx_env->gs_alpha;
    sceGsSetDefTexEnv(&tx_env->gs_tex, 0, tbp0, D_0013A230 / 0x40, SCE_GS_PSMCT32, 10, 8, 0, 0, 0, 0, 1);
    sceGsSetDefAlphaEnv(alpha, 0);
    *(u64 *)&alpha->alpha1 = SCE_GS_SET_ALPHA(SCE_GS_ALPHA_CS, SCE_GS_ALPHA_CD, SCE_GS_ALPHA_AS, SCE_GS_ALPHA_CD, 0);
    *(u64 *)&tx_env->gs_test = SCE_GS_SET_TEST(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_FALSE, SCE_GS_ALPHA_ALWAYS);
    tx_env->gs_test1addr = SCE_GS_TEST_1;
    *(u64 *)&tx_env->gs_tex.tex11 = SCE_GS_SET_TEX1(1, 0, SCE_GS_LINEAR, SCE_GS_LINEAR, 0, 0, 0);
    *(u64 *)&tx_env->gs_tex.clamp1 = SCE_GS_SET_CLAMP(SCE_GS_REPEAT, SCE_GS_REPEAT, 0, 0, 0, 0);
    *(u64 *)&tx_env->gs_alpha.fba1 = SCE_GS_SET_FBA(0);
    sceGsSyncPath(0, 0);
    sceGsPutDrawEnv(&tx_env->giftag);
}

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

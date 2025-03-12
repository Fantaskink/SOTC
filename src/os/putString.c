#include "putString.h"
#include "common.h"
#include "gcc/ctype.h"
#include "sdk/ee/eestruct.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

// data
TexEnv D_001320D0 = {
    .giftag = {
        .NLOOP = 9,
        .EOP = 1,
        .NREG = 1,
        .REGS0 = SCE_GIF_PACKED_AD,
    },
};

DrawEnv D_00132170 = {
    .giftag = {
        .NLOOP = 8,
        .EOP = 1,
        .NREG = 1,
        .REGS0 = SCE_GIF_PACKED_AD,
    },
};

TexEnv D_00132200 = {
    .giftag = {
        .NLOOP = 9,
        .EOP = 1,
        .NREG = 1,
        .REGS0 = SCE_GIF_PACKED_AD,
    },
};

TexEnv D_001322A0 = {
    .giftag = {
        .NLOOP = 2,
        .EOP = 1,
        .NREG = 1,
        .REGS0 = SCE_GIF_PACKED_AD,
    },
};

sceGifTag D_00132340 = {
    .NLOOP = 0,
    .EOP = 1,
    .FLG = 1,
    .NREG = 1,
    .REGS0 = SCE_GS_XYZF2,
};

sceGifTag D_00132350 = {
    .NLOOP = 0,
    .EOP = 1,
    .FLG = 1,
    .NREG = 2,
    .REGS0 = SCE_GS_UV,
    .REGS1 = SCE_GS_XYZF2,
};

sceGifTag D_00132360 = {
    .NLOOP = 0,
    .EOP = 1,
    .FLG = 1,
    .NREG = 1,
    .REGS0 = SCE_GS_XYZF2,
};

// rodata
extern u8 D_00137540[256][8]; // Font raster, 1bpp

static const sceGifTag D_00137D40 = {
    .NLOOP = 0,
    .EOP = 1,
    .FLG = 1,
    .NREG = 2,
    .REGS0 = SCE_GS_PRIM,
    .REGS1 = SCE_GS_RGBAQ,
};

static const sceGifTag D_00137D50 = {
    .NLOOP = 0,
    .EOP = 1,
    .FLG = 1,
    .NREG = 2,
    .REGS0 = SCE_GS_PRIM,
    .REGS1 = SCE_GS_RGBAQ,
};

// sdata
s32 D_0013A230 = 0;
s32 D_0013A234 = 0;
s32 D_0013A238 = 0;
s32 D_0013A23C = 0;
s32 D_0013A240 = 0;
u32 D_0013A244 = 0;
f32 D_0013A248 = 1.0f;
f32 D_0013A24C = 1.0f;
s32 D_0013A250 = 0xFF;
s32 D_0013A254 = 0xFF;
s32 D_0013A258 = 0xFF;
struct t_PutStringFBChar D_0013A260 = {0, PUTSTR_CH_SPACE};
s32 D_0013A268 = 0;
s32 D_0013A26C = 0;
s32 D_0013A270 = 0;
u32 D_0013A274 = 0;
s32 D_0013A278 = 0;
s32 D_0013A27C = 0;
s32 D_0013A280 = 0;

// bss
static sceGsDBuff D_0013ECE0;
static struct t_PutStringFBChar D_0013EF10[PUT_STRING_FB_HGHT][PUT_STRING_FB_WDTH];

void PutChar(PutStringColor color, char ch)
{
    D_0013EF10[D_0013A270][D_0013A26C].color = color;
    D_0013EF10[D_0013A270][D_0013A26C].ch = ch;
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

inline void SetPrimColorTex(s32 prim_type, s32 r, s32 g, s32 b, s32 a, s32 use_uv)
{
    sceVif1Packet pkt;
    sceVif1Packet *pPkt;
    sceDmaChan *dmaVif;

    pPkt = &pkt;
    sceVif1PkInit(pPkt, (u128 *)(0x70000000 | (D_0013A244 << 0xd)));
    sceVif1PkReset(pPkt);
    sceVif1PkCnt(pPkt, 0);
    sceVif1PkOpenDirectCode(pPkt, 0);

    D_0013A244 = (D_0013A244 + 1) & 1;

    sceVif1PkOpenGifTag(&pkt, *(u128 *)&D_00137D50);
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_PRIM(prim_type, 0, 1, 0, 0, 0, use_uv != 0, 0, 0));
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_RGBAQ(r, g, b, a, *(u32 *)&D_0013A24C));
    sceVif1PkCloseGifTag(&pkt);
    sceVif1PkCloseDirectCode(&pkt);
    sceVif1PkEnd(&pkt, 0);
    sceVif1PkTerminate(&pkt);

    sceGsSyncPath(0, 0);

    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;
    sceDmaSend(dmaVif, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

inline void SetPrimColor(s32 prim_type, s32 r, s32 g, s32 b, s32 a)
{
    sceVif1Packet pkt;
    sceVif1Packet *pPkt;
    sceDmaChan *dmaVif;

    pPkt = &pkt;
    sceVif1PkInit(pPkt, (u128 *)(0x70000000 | (D_0013A244 << 0xd)));
    sceVif1PkReset(pPkt);
    sceVif1PkCnt(pPkt, 0);
    sceVif1PkOpenDirectCode(pPkt, 0);

    D_0013A244 = (D_0013A244 + 1) & 1;

    sceVif1PkOpenGifTag(&pkt, *(u128 *)&D_00137D40);
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_PRIM(prim_type, 0, 0, 0, 1, 0, 0, 0, 0));
    sceVif1PkAddGsData(&pkt, SCE_GS_SET_RGBAQ(r, g, b, a, *(u32 *)&D_0013A248));
    sceVif1PkCloseGifTag(&pkt);
    sceVif1PkCloseDirectCode(&pkt);
    sceVif1PkEnd(&pkt, 0);
    sceVif1PkTerminate(&pkt);

    sceGsSyncPath(0, 0);

    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;
    sceDmaSend(dmaVif, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

void PutFont(s32 arg0, s32 arg1, s32 arg2)
{
    sceVif1Packet pkt;
    sceDmaChan *dmaVif;
    s32 i;
    s32 j;

    SetPrimColor(SCE_GS_PRIM_POINT, D_0013A250, D_0013A254, D_0013A258, 0x80);

    sceVif1PkInit(&pkt, (u128 *)((D_0013A244 << 0xD) | 0x70000000));
    sceVif1PkReset(&pkt);
    sceVif1PkCnt(&pkt, 0);
    sceVif1PkOpenDirectCode(&pkt, 0);
    D_0013A244 = (D_0013A244 + 1) & 1;
    sceVif1PkOpenGifTag(&pkt, *(u128 *)&D_00132340);

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if ((D_00137540[arg0][i] >> j) & 1)
            {
                sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg1 + j + 0x800) * 0x10, (arg2 + i + 0x800) << 0x4, 0x800000, 0));
                sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg1 + j + 0x800) * 0x10, ((arg2 + i + 0x800) << 0x4) - 0x10, 0x800000, 0));
            }
        }
    }
    sceVif1PkCloseGifTag(&pkt);
    sceVif1PkCloseDirectCode(&pkt);
    sceVif1PkEnd(&pkt, 0);
    sceVif1PkTerminate(&pkt);
    sceGsSyncPath(0, 0);
    dmaVif = sceDmaGetChan(1);
    dmaVif->chcr.TTE = 1;
    sceDmaSend(dmaVif, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
}

void _putString(PutStringColor color, char *strIn)
{
    s32 chIx; // character ptr/ix in the string

    LoaderSysPrintf(strIn); // They seem very sure there won't be any printf() format specifiers inside. The safe way is printf("%s", strIn).

    // Why did they leave the strlen in the loop to be called every iter?
    for (chIx = 0; chIx < strlen(strIn); chIx++)
    {
        if (strIn[chIx] == '\t')
        {
            PutChar(0xFFFFFF00, PUTSTR_CH_SPACE);
            D_0013A26C += 2;
        }
        else if (strIn[chIx] == '\n')
        {
            PutChar(0xFFFFFF00, PUTSTR_CH_SPACE);
            D_0013A270++;
            D_0013A26C = 0;
            D_0013A274 = 0;
        }
        else
        {
            // regular symbols:
            D_0013EF10[D_0013A270][D_0013A26C].color = color;

            if (strIn[chIx] == '\'')
            {
                D_0013A278 = !D_0013A278;
            }
            if (strIn[chIx] == '"')
            {
                D_0013A27C = !D_0013A27C;
            }

            D_0013EF10[D_0013A270][D_0013A26C].ch = (D_0013A27C) ? strIn[chIx] : toupper(strIn[chIx]);
            D_0013A26C++;
        }

        if (D_0013A26C >= PUT_STRING_FB_WDTH)
        {
            D_0013A26C = PUT_STRING_FB_WDTH - 1;
        }

        if (D_0013A270 >= PUT_STRING_FB_HGHT)
        {
            ScrollDisplay();
            D_0013A270 = PUT_STRING_FB_HGHT - 1; // keep at the last line
        }
    }
}

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

// Send the screen character buffer to the display.
void func_00105A60(void)
{
    s32 ix, iy, sx, s5;
    PutStringColor color;
    s32 xb;
    s32 j;

    s5 = PUT_STRING_FB_WDTH;

    for (iy = 0; iy < PUT_STRING_FB_HGHT; iy++)
    {
        char c = PUTSTR_CH_SPACE;

        for (ix = PUT_STRING_FB_WDTH - 1;; ix--)
        {
            if (D_0013EF10[iy][ix].ch != PUTSTR_CH_SPACE)
                break;
            if (ix < 0)
                break;
        }

        ix += 1;
        if (s5 < ix)
            s5 = ix;

        xb = 0;
        if (ix >= PUT_STRING_FB_WDTH)
        {
            xb = ((ix - D_0013A268 - PUT_STRING_FB_WDTH) >= 0) ? D_0013A268 : (ix - PUT_STRING_FB_WDTH);
        }

        sx = -30;
        for (j = 0; j < PUT_STRING_FB_WDTH; j++)
        {
            if (D_0013EF10[iy][j + xb].ch != c)
            {
                color = D_0013EF10[iy][j + xb].color;
                D_0013A250 = PUTSTR_COL_GET_R(color) * 0.8f;
                D_0013A254 = PUTSTR_COL_GET_G(color) * 0.8f;
                D_0013A258 = PUTSTR_COL_GET_B(color) * 0.8f;
                PutFont(D_0013EF10[iy][j + xb].ch, sx * 10, iy * 10 - 200);
            }
            sx++;
        }
    }

    // seems correct way
    if ((s5 - D_0013A268 - PUT_STRING_FB_WDTH) < 0)
        D_0013A268 = s5 - PUT_STRING_FB_WDTH;
}

void func_00105C50(void)
{
    if ((D_0013A274 >> 4) & 1)
    {
        PutChar(0xFFFFFF00, PUTSTR_CH_SPACE);
    }
    else
    {
        PutChar(0xFFFFFF00, PUTSTR_CH_CURSOR);
    }
    D_0013A274++;
    D_0013A274 = D_0013A274 & 0xFF;
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

void Sync(void)
{
    FlushCache(0);
    sceGsSyncPath(1, 0);
    D_0013A240 = sceGsSyncV(0);
    D_0013A238 = (D_0013A23C + 1) & 1;
    D_0013A23C++;
    sceGsSwapDBuff(&D_0013ECE0, D_0013A238);
    RestoreNormalDrawEnvironment(&D_0013ECE0, D_0013A238, D_0013A240);
    SetDrawEnvironment(1);
}

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

void LoaderSysDrawSprite(xypair *arg0, uvpair *arg1, rgba32 *color, s32 arg3, s32 arg4)
{
    sceVif1Packet pkt;
    sceDmaChan *dmaVif2;
    sceDmaChan *dmaVif3;

    if (arg4 != 0)
    {
        SetPrimColorTex(SCE_GS_PRIM_SPRITE, color->r, color->g, color->b, color->a, 1);

        sceVif1PkInit(&pkt, (u128 *)((D_0013A244 << 0xD) | 0x70000000));
        sceVif1PkReset(&pkt);
        sceVif1PkCnt(&pkt, 0);
        sceVif1PkOpenDirectCode(&pkt, 0);
        D_0013A244 = (D_0013A244 + 1) & 1;
        sceVif1PkOpenGifTag(&pkt, *(u128 *)&D_00132350);
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_UV(arg1->u0, arg1->v0));
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg0->x0 + 0x800) << 4, (arg0->y0 + 0x800) << 4, 0x800000, 0));
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_UV(arg1->u1 << 4, arg1->v1 << 4));
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg0->x1 + 0x800) << 4, (arg0->y1 + 0x800) << 4, 0x800000, 0));
        sceVif1PkCloseGifTag(&pkt);
        sceVif1PkCloseDirectCode(&pkt);
        sceVif1PkEnd(&pkt, 0);
        sceVif1PkTerminate(&pkt);
        sceGsSyncPath(0, 0);
        dmaVif2 = sceDmaGetChan(1);
        dmaVif2->chcr.TTE = 1;
        sceDmaSend(dmaVif2, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
    }
    else
    {
        SetPrimColorTex(SCE_GS_PRIM_SPRITE, color->r, color->g, color->b, color->a, 1);

        sceVif1PkInit(&pkt, (u128 *)((D_0013A244 << 0xD) | 0x70000000));
        sceVif1PkReset(&pkt);
        sceVif1PkCnt(&pkt, 0);
        sceVif1PkOpenDirectCode(&pkt, 0);
        D_0013A244 = (D_0013A244 + 1) & 1;
        sceVif1PkOpenGifTag(&pkt, *(u128 *)&D_00132360);
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg0->x0 + 0x800) << 4, (arg0->y0 + 0x800) << 4, 0x800000, 0));
        sceVif1PkAddGsData(&pkt, SCE_GS_SET_XYZF2((arg0->x1 + 0x800) << 4, (arg0->y1 + 0x800) << 4, 0x800000, 0));
        sceVif1PkCloseGifTag(&pkt);
        sceVif1PkCloseDirectCode(&pkt);
        sceVif1PkEnd(&pkt, 0);
        sceVif1PkTerminate(&pkt);
        sceGsSyncPath(0, 0);
        dmaVif3 = sceDmaGetChan(1);
        dmaVif3->chcr.TTE = 1;
        sceDmaSend(dmaVif3, (u128 *)(0x80000000 | ((int)pkt.pBase & 0x3FF0)));
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
    ClearDisplay();
    SetLocate(0, 0);
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

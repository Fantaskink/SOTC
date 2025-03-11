#ifndef _PUTSTRING_H_
#define _PUTSTRING_H_

#include "common.h"
#include "sdk/ee/eetypes.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

#define PUT_STRING_FB_WDTH (60)
#define PUT_STRING_FB_HGHT (40)

#define PUTSTR_COL_MAKE(r, g, b, a) ((PutStringColor)(((r) & 0xFF) << 24 | ((g) & 0xFF) << 16 | ((b) & 0xFF) << 8 | ((a) & 0xFF) << 0))
#define PUTSTR_COL_MAKEW(wi) ((PutStringColor)(wi))
#define PUTSTR_COL_GET_R(cl) (((int)(cl) >> 24) & 0xFF)
#define PUTSTR_COL_GET_G(cl) (((int)(cl) >> 16) & 0xFF)
#define PUTSTR_COL_GET_B(cl) (((int)(cl) >> 8) & 0xFF)
#define PUTSTR_COL_GET_A(cl) (((int)(cl) >> 0) & 0xFF)

// Number of calls to PutString(), after which to send Sync(), ExecBaseProc() (=Flush perhaps) (at the last call of them).
#define PUT_STRING_SYNC_ITER (5)

typedef s32 PutStringColor;
struct t_PutStringFBChar
{
    PutStringColor color; // 00
    char ch;              // 04
}; // = 0x08 / 0x05

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

// TODO: better names for these
typedef struct rgba32
{
    s32 r;
    s32 g;
    s32 b;
    s32 a;
} rgba32;

typedef struct uvpair
{
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
} uvpair;

typedef struct xypair
{
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} xypair;

extern TexEnv D_001320D0;
extern DrawEnv D_00132170;
extern TexEnv D_00132200;
extern TexEnv D_001322A0;
extern u128 D_00132340;
extern u128 D_00132350;
extern u128 D_00132360;
extern u8 D_00137540[100][8];

extern s32 D_0013A230;
extern s32 D_0013A234;
extern s32 D_0013A238;
extern s32 D_0013A23C;
extern s32 D_0013A240;
extern u128 D_00137D40;
extern u128 D_00137D50;
extern u32 D_0013A244;
extern u32 D_0013A248;
extern u32 D_0013A24C;
extern s32 D_0013A250;
extern s32 D_0013A254;
extern s32 D_0013A258;
extern struct t_PutStringFBChar D_0013A260;
extern s32 D_0013A268;
extern s32 D_0013A26C;
extern s32 D_0013A270;
extern u32 D_0013A274;
extern s32 D_0013A27C;
extern s32 D_0013A278;
extern s32 D_0013A280;
extern sceGsDBuff D_0013ECE0;
extern struct t_PutStringFBChar D_0013EF10[PUT_STRING_FB_HGHT][PUT_STRING_FB_WDTH];

void _putString(s32 color, char *arg1);
void PutString(PutStringColor color, const char *format, ...);
void PutStringS(PutStringColor color, const char *format, ...);
void func_00105A60(void);
void func_00105C50(void);
void Sync(void);
void ReinitDisp(void);
void LoaderSysDrawSprite(xypair *arg0, uvpair *arg1, rgba32 *color, s32 arg3, s32 arg4);
void ExecBaseProc(void);
void SetLocate(s32 x, s32 y);
void ScrollDisplay(void);
void ClearDisplay(void);
void InitDisp(void);
void RestoreNormalDrawEnvironment(sceGsDBuff *dbuff, s32 arg1, s32 half_off);
void SetTextureWithFrameBuffer(s16 tbp0);
void SetTexDrawEnvironment(s32 arg0);
void SetDrawnTextureEnvironment(s16 tbp0);
void SetDrawEnvironment(s32 mode);
void SetPrimColor(s32 prim_type, s32 r, s32 g, s32 b, s32 a);
void SetPrimColorTex(s32 prim_type, s32 r, s32 g, s32 b, s32 a, s32 use_uv);
void PutChar(PutStringColor color, char ch);

// TODO: remove these
static inline void __inlined_ClearDisplay(void)
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

static inline void __inlined_SetPrimColorTex(s32 prim_type, s32 r, s32 g, s32 b, s32 a, s32 use_uv)
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

static inline void __inlined_SetPrimColor(s32 prim_type, s32 r, s32 g, s32 b, s32 a)
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

static inline void __inlined_SetLocate(s32 x, s32 y)
{
    D_0013A26C = x;
    D_0013A270 = y;
}

static inline void __inlined_SetDrawEnvironment(s32 mode)
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

static inline void __inlined_RestoreNormalDrawEnvironment(sceGsDBuff *dbuff, s32 arg1, s32 half_off)
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

static inline void __inlined_PutChar(PutStringColor color, char ch)
{
    D_0013EF10[D_0013A270][D_0013A26C].color = color;
    D_0013EF10[D_0013A270][D_0013A26C].ch = ch;
}

static inline void __inlined_ScrollDisplay(void)
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

#endif /* _PUTSTRING_H_ */

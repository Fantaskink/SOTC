#ifndef _PUTSTRING_H_
#define _PUTSTRING_H_

#include "common.h"
#include "sdk/ee/eetypes.h"
#include "sdk/ee/libgraph.h"

#define PUT_STRING_FB_WDTH (60)
#define PUT_STRING_FB_HGHT (40)

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

extern TexEnv D_001320D0;
extern DrawEnv D_00132170;
extern TexEnv D_00132200;
extern TexEnv D_001322A0;

extern s32 D_0013A230;
extern s32 D_0013A234;
extern s32 D_0013A238;
extern u128 D_00137D40;
extern u128 D_00137D50;
extern u32 D_0013A244;
extern u32 D_0013A248;
extern u32 D_0013A24C;
extern struct t_PutStringFBChar D_0013A260;
extern s32 D_0013A26C;
extern s32 D_0013A270;
extern s32 D_0013A280;
extern sceGsDBuff D_0013ECE0;
extern struct t_PutStringFBChar D_0013EF10[PUT_STRING_FB_HGHT][PUT_STRING_FB_WDTH];

void _putString(s32 color, char *arg1);
void PutString(PutStringColor color, const char *format, ...);
void PutStringS(PutStringColor color, const char *format, ...);
void Sync(void);
void ReinitDisp(void);
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
static inline void __inlined_ClearDisplay(void) {
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

static inline void __inlined_SetLocate(s32 x, s32 y)
{
    D_0013A26C = x;
    D_0013A270 = y;
}

#endif /* _PUTSTRING_H_ */

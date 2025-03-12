#ifndef _PUTSTRING_H_
#define _PUTSTRING_H_

#include "common.h"
#include "sdk/ee/eetypes.h"
#include "sdk/ee/libdma.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/libvifpk.h"

// TODO: Probably makes more sense to have these on their own header
#define SCREEN_WIDTH            (640)
#define SCREEN_HEIGHT           (448)
#define SCREEN_CENTERX          (2048)
#define SCREEN_CENTERY          (2048)
#define GS_X_COORD(x)           (((x) + SCREEN_CENTERX) << 4)
#define GS_Y_COORD(y)           (((y) + SCREEN_CENTERY) << 4)

#define PUT_STRING_FB_WDTH (60)
#define PUT_STRING_FB_HGHT (40)

#define PUTSTR_COL_MAKE(r, g, b, a) ((PutStringColor)(((r) & 0xFF) << 24 | ((g) & 0xFF) << 16 | ((b) & 0xFF) << 8 | ((a) & 0xFF) << 0))
#define PUTSTR_COL_MAKEW(wi) ((PutStringColor)(wi))
#define PUTSTR_COL_GET_R(cl) (((int)(cl) >> 24) & 0xFF)
#define PUTSTR_COL_GET_G(cl) (((int)(cl) >> 16) & 0xFF)
#define PUTSTR_COL_GET_B(cl) (((int)(cl) >> 8) & 0xFF)
#define PUTSTR_COL_GET_A(cl) (((int)(cl) >> 0) & 0xFF)

// Number of calls to PutString(), after which to send Sync(), ExecBaseProc() (=Flush perhaps) (at the last call of them).
#define PUT_STRING_SYNC_ITER (4)

// TODO: Import all chars in C struct form and add enums with what each char is maybe. Or at least the special ones, as the others should be ASCII.
enum
{
    PUTSTR_CH_CURSOR = 0x12, // Black rectangle.
    PUTSTR_CH_SPACE = 0x20,  // ' '
};

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

#endif /* _PUTSTRING_H_ */

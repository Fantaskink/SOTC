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

#endif /* _PUTSTRING_H_ */

#ifndef _PUTSTRING_H_
#define _PUTSTRING_H_

#include "common.h"

void _putString(s32 color, char* arg1);
void PutString(s32 color, const char* format, ...);
void PutStringS(s32 color, const char* format, ...);
void Sync(void);
void ReinitDisp(void);
void ExecBaseProc(void);
void SetLocate(s32 arg0, s32 arg1);
void ScrollDisplay(void);
void ClearDisplay(void);
void InitDisp(void);
void RestoreNormalDrawEnvironment(s32 arg0, s32 arg1, s32 arg2);
void SetTextureWithFrameBuffer(s16 arg0);
void SetTexDrawEnvironment(s32 arg0);
void SetDrawnTextureEnvironment(s16 arg0);
void SetDrawEnvironment(u32 arg0);
void SetPrimColor(s32 data, s32 r, s32 g, s32 b, s32 a) ;

#endif /* _PUTSTRING_H_ */

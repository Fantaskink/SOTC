#ifndef LOADERSYSEXCEPTION_H
#define LOADERSYSEXCEPTION_H

#include "common.h"

typedef struct unk_except_s {
    u32 value;
    char* name;
} unk_except_s;

void func_00101EC0(s32 except_code, u32 cause, u32 epc, u32 bva, u32 bpa, unk_except_s *ctx, u32 mode);
void InitException(void);
void setCop0Epc(int epc);
void func_001021E0(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128 *gpr);
void func_00102360(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128 *gpr);

#endif /* LOADERSYSEXCEPTION_H */

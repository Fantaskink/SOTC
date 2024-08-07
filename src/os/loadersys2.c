#include "common.h"

extern u32 D_00139F04; // heap pointer

#define HEAP_START D_00139F04

INCLUDE_ASM(const s32, "os/loadersys2", func_00100A58);

INCLUDE_ASM(const s32, "os/loadersys2", LoaderSysJumpRecoverPointNoStateSetting);

INCLUDE_ASM(const s32, "os/loadersys2", LoaderSysJumpRecoverPoint);

INCLUDE_ASM(const s32, "os/loadersys2", func_00100D48);

INCLUDE_ASM(const s32, "os/loadersys2", MoveElf);

INCLUDE_ASM(const s32, "os/loadersys2", func_001013C8);

INCLUDE_ASM(const s32, "os/loadersys2", LoaderSysRelocateOnlineElfInfo);

INCLUDE_ASM(const s32, "os/loadersys2", RelocateCode);

INCLUDE_ASM(const s32, "os/loadersys2", FreeDecodedSection);

INCLUDE_ASM(const s32, "os/loadersys2", RelocateSelfSymbol);

INCLUDE_ASM(const s32, "os/loadersys2", DisposeRelocationElement);

void SetHeapStartPoint(u32 start_address)
{
    HEAP_START = (start_address + 0xF) & ~0xF;
}

int GetHeapCurrentPoint(void)
{
    return HEAP_START;
}

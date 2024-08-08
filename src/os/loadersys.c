#include "common.h"

INCLUDE_ASM(const s32, "os/loadersys", ResolveRelocation);

INCLUDE_ASM(const s32, "os/loadersys", DecodeSection);

INCLUDE_ASM(const s32, "os/loadersys", RelocateElfInfoHeader);

INCLUDE_ASM(const s32, "os/loadersys", OutputLinkerScriptFile);

INCLUDE_ASM(const s32, "os/loadersys", func_00100A58);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysJumpRecoverPointNoStateSetting);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysJumpRecoverPoint);

INCLUDE_ASM(const s32, "os/loadersys", func_00100D48);

INCLUDE_ASM(const s32, "os/loadersys", MoveElf);

INCLUDE_ASM(const s32, "os/loadersys", func_001013C8);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRelocateOnlineElfInfo);

INCLUDE_ASM(const s32, "os/loadersys", RelocateCode);

INCLUDE_ASM(const s32, "os/loadersys", FreeDecodedSection);

INCLUDE_ASM(const s32, "os/loadersys", RelocateSelfSymbol);

INCLUDE_ASM(const s32, "os/loadersys", DisposeRelocationElement);

INCLUDE_ASM(const s32, "os/loadersys", SetHeapStartPoint);

INCLUDE_ASM(const s32, "os/loadersys", GetHeapCurrentPoint);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysResetSystem);

INCLUDE_ASM(const s32, "os/loadersys", mallocAlignMempool);

INCLUDE_ASM(const s32, "os/loadersys", mallocAlign0x100Mempool);

INCLUDE_ASM(const s32, "os/loadersys", _checkExistString);

INCLUDE_ASM(const s32, "os/loadersys", func_00101B88);

INCLUDE_ASM(const s32, "os/loadersys", _execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys", execProgWithThread);

INCLUDE_ASM(const s32, "os/loadersys", func_00101EC0);

INCLUDE_ASM(const s32, "os/loadersys", InitException);

INCLUDE_ASM(const s32, "os/loadersys", setCop0Epc);

INCLUDE_ASM(const s32, "os/loadersys", func_001021E0);

INCLUDE_ASM(const s32, "os/loadersys", func_00102360);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalIntcHandler);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalSema);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysExecuteRecoveryFirstProcess);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalThreadList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalSemaList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysEntryExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteExternalThreadList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteExternalSemaList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitExternalSemaList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitExternalThreadList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysExternalThreadListCallBack);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysChangeExternalThreadPriorityExceptMe);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalThread);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalThreadExceptMe);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitExternalIopMemoryList);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDeleteAllExternalIopMemory);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysPrintf);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysLoadIopModuleFromEEBuffer);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysCheckCDBootMode);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysPutString);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFOpen);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFClose);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFSeek);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFSeek64);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFRead);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysFWrite);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRemove);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRmdir);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysMkdir);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysGetstat);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysChstat);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysGetMemoryInfo);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysDumpIopModuleIdentifiers);

void LoaderSysSendAbort(void) {
}

void LoaderSysFlushPrint(void) {
}

INCLUDE_ASM(const s32, "os/loadersys", checkHookDesc);

INCLUDE_ASM(const s32, "os/loadersys", setNewIopIdentifier);

INCLUDE_ASM(const s32, "os/loadersys", func_001033B0);

INCLUDE_ASM(const s32, "os/loadersys", loaderLoop);

INCLUDE_ASM(const s32, "os/loadersys", main);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysGetBootArg);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysLoadIopModule);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysUnloadIopModuleByName);

void LoaderSysHookPoint(void) {
}

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysRebootIop);

INCLUDE_ASM(const s32, "os/loadersys", loaderExecResetCallback);

INCLUDE_ASM(const s32, "os/loadersys", loaderSetResetCallback);

INCLUDE_ASM(const s32, "os/loadersys", memprintf);

INCLUDE_ASM(const s32, "os/loadersys", imemprintf);

void initmemprintf(void) {
}

INCLUDE_ASM(const s32, "os/loadersys", func_00104090);

INCLUDE_ASM(const s32, "os/loadersys", func_00104428);

INCLUDE_ASM(const s32, "os/loadersys", func_00104668);

INCLUDE_ASM(const s32, "os/loadersys", func_00104818);

INCLUDE_ASM(const s32, "os/loadersys", LoaderSysInitTCP);

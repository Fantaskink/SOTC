#include "common.h"

INCLUDE_ASM(const s32, "os\loadersys", ResolveRelocation);

INCLUDE_ASM(const s32, "os\loadersys", DecodeSection);

INCLUDE_ASM(const s32, "os\loadersys", RelocateElfInfoHeader);

INCLUDE_ASM(const s32, "os\loadersys", OutputLinkerScriptFile);

INCLUDE_ASM(const s32, "os\loadersys", func_00100A58);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysJumpRecoverPointNoStateSetting);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysJumpRecoverPoint);

INCLUDE_ASM(const s32, "os\loadersys", func_00100D48);

INCLUDE_ASM(const s32, "os\loadersys", MoveElf);

INCLUDE_ASM(const s32, "os\loadersys", func_001013C8);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysRelocateOnlineElfInfo);

INCLUDE_ASM(const s32, "os\loadersys", RelocateCode);

INCLUDE_ASM(const s32, "os\loadersys", FreeDecodedSection);

INCLUDE_ASM(const s32, "os\loadersys", RelocateSelfSymbol);

INCLUDE_ASM(const s32, "os\loadersys", DisposeRelocationElement);

INCLUDE_ASM(const s32, "os\loadersys", SetHeapStartPoint);

extern s32 D_00139F04;

int GetHeapCurrentPoint(void)
{
    return D_00139F04;
}

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysResetSystem);

INCLUDE_ASM(const s32, "os\loadersys", mallocAlignMempool);

INCLUDE_ASM(const s32, "os\loadersys", mallocAlign0x100Mempool);

INCLUDE_ASM(const s32, "os\loadersys", _checkExistString);

INCLUDE_ASM(const s32, "os\loadersys", func_00101B88);

INCLUDE_ASM(const s32, "os\loadersys", _execProgWithThread);

INCLUDE_ASM(const s32, "os\loadersys", execProgWithThread);

INCLUDE_ASM(const s32, "os\loadersys", func_00101EC0);

INCLUDE_ASM(const s32, "os\loadersys", InitException);

INCLUDE_ASM(const s32, "os\loadersys", setCop0Epc);

INCLUDE_ASM(const s32, "os\loadersys", func_001021E0);

INCLUDE_ASM(const s32, "os\loadersys", func_00102360);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteAllExternalIntcHandler);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteAllExternalSema);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysExecuteRecoveryFirstProcess);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysEntryExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysEntryExternalThreadList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysEntryExternalSemaList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysEntryExternalIopMemoryList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteExternalIntcHandlerList);

extern s32 D_0013B910[];

s32 LoaderSysDeleteExternalThreadList(s32 threadId)
{
    s32 index;
    s32 *threadListEntry;

    // Assume D_0013B910 is a pointer to the start of the thread list array
    threadListEntry = D_0013B910;

    // Loop through the thread list to find the specified threadId
    for (index = 0; index < 256; index++)
    {
        if (*threadListEntry == threadId)
        {
            // If found, mark the entry as deleted by setting it to -1
            *threadListEntry = -1;
            // Return the threadId indicating successful deletion
            return threadId;
        }
        // Move to the next entry in the thread list
        threadListEntry++;
    }

    // If the threadId was not found, return -1 indicating failure
    return -1;
}

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteExternalSemaList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteExternalIopMemoryList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysInitExternalIntcHandlerList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysInitExternalSemaList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysInitExternalThreadList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysExternalThreadListCallBack);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysChangeExteranalThreadPriorityExceptMe);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteAllExternalThread);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteAllExternalThreadExceptMe);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysInitExternalIopMemoryList);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDeleteAllExternalIopMemory);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysPrintf);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysLoadIopModuleFromEEBuffer);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysCheckCDBootMode);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysPutString);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFOpen);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFClose);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFSeek);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFSeek64);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFRead);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysFWrite);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysRemove);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysRmdir);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysMkdir);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysGetstat);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysChstat);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysGetMemoryInfo);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDumpIopModuleIdentifiers);

void LoaderSysSendAbort(void)
{
}

void LoaderSysFlushPrint(void)
{
}

s32 checkHookDesc(char *hook_desc)
{
    s64 lVar1;
    s32 iVar2;

    lVar1 = strncmp(hook_desc, "host0:", 6);
    iVar2 = (int)hook_desc + 6;
    if (lVar1 == 0)
    {
        return iVar2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "os\loadersys", setNewIopIdentifier);

INCLUDE_ASM(const s32, "os\loadersys", func_001033B0);

INCLUDE_ASM(const s32, "os\loadersys", loaderLoop);

INCLUDE_ASM(const s32, "os\loadersys", main);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysGetBootArg);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysLoadIopModule);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysUnloadIopModuleByName);

void LoaderSysHookPoint(void)
{
}

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysRebootIop);

INCLUDE_ASM(const s32, "os\loadersys", loaderExecResetCallback);

INCLUDE_ASM(const s32, "os\loadersys", loaderSetResetCallback);

INCLUDE_ASM(const s32, "os\loadersys", memprintf);

INCLUDE_ASM(const s32, "os\loadersys", imemprintf);

void initmemprintf(void)
{
}

INCLUDE_ASM(const s32, "os\loadersys", func_00104090);

INCLUDE_ASM(const s32, "os\loadersys", func_00104428);

INCLUDE_ASM(const s32, "os\loadersys", func_00104668);

INCLUDE_ASM(const s32, "os\loadersys", func_00104818);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysInitTCP);

INCLUDE_ASM(const s32, "os\loadersys", padSysTickProc);

INCLUDE_ASM(const s32, "os\loadersys", padsysInit);

INCLUDE_ASM(const s32, "os\loadersys", padSysGet);

INCLUDE_ASM(const s32, "os\loadersys", padSysReadForLoader);

INCLUDE_ASM(const s32, "os\loadersys", padSysVibSetAcrParam);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysSetPowerOffCallBackFunc);

INCLUDE_ASM(const s32, "os\loadersys", PreparePowerOff);

INCLUDE_ASM(const s32, "os\loadersys", func_00104FF0);

INCLUDE_ASM(const s32, "os\loadersys", usbSerialSysPutString);

INCLUDE_ASM(const s32, "os\loadersys", usbSerialSysPrintf);

INCLUDE_ASM(const s32, "os\loadersys", usbSerialSysInit);

INCLUDE_ASM(const s32, "os\loadersys", func_001053F0);

INCLUDE_ASM(const s32, "os\loadersys", PutFont);

INCLUDE_ASM(const s32, "os\loadersys", _putString);

INCLUDE_ASM(const s32, "os\loadersys", PutString);

INCLUDE_ASM(const s32, "os\loadersys", PutStringS);

INCLUDE_ASM(const s32, "os\loadersys", func_00105A60);

INCLUDE_ASM(const s32, "os\loadersys", func_00105C50);

INCLUDE_ASM(const s32, "os\loadersys", Sync);

INCLUDE_ASM(const s32, "os\loadersys", ReinitDisp);

INCLUDE_ASM(const s32, "os\loadersys", LoaderSysDrawSprite);

INCLUDE_ASM(const s32, "os\loadersys", ExecBaseProc);

INCLUDE_ASM(const s32, "os\loadersys", SetLocate);

INCLUDE_ASM(const s32, "os\loadersys", ScrollDisplay);

INCLUDE_ASM(const s32, "os\loadersys", ClearDisplay);

INCLUDE_ASM(const s32, "os\loadersys", InitDisp);

INCLUDE_ASM(const s32, "os\loadersys", RestoreNormalDrawEnvironment);

INCLUDE_ASM(const s32, "os\loadersys", SetTextureWithFrameBuffer);

INCLUDE_ASM(const s32, "os\loadersys", SetTexDrawEnvironment);

INCLUDE_ASM(const s32, "os\loadersys", SetDrawnTextureEnvironment);

INCLUDE_ASM(const s32, "os\loadersys", SetDrawEnvironment);

INCLUDE_ASM(const s32, "os\loadersys", SetPrimColor);

INCLUDE_ASM(const s32, "os\loadersys", SetPrimColorTex);

INCLUDE_ASM(const s32, "os\loadersys", PutChar);

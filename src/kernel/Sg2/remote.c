#include "common.h"

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteSend);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteSendDmaCom);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteDmaTransStatus);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteGetAdpStmAdr);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteGetPcmStmTransOffset);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteGetSlotReleaseLv);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteGetDspChangeStatus);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteSyncStatus);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteTickProc);

INCLUDE_ASM("asm/kernel/nonmatchings/Sg2/remote", Sg2RemoteInit);

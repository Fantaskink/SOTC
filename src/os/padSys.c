#include "common.h"
#include "padSys.h"
#include "sdk/ee/libpad2.h"

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysTickProc);

void padsysInit(void)
{
    scePad2SocketParam pad_socket_param;
    struct t_padSysData *temp;
    int i;

    scePad2Init(0);

    for (i = 0; i < 2; i++)
    {
        pad_socket_param.port = i != 0;
        pad_socket_param.option = 2;
        pad_socket_param.slot = 0;
        temp = &D_0013D9C0[i];
        temp->socket = scePad2CreateSocket(&pad_socket_param, (void *)temp);
        temp->gotProfile = 0;
    }
}

struct t_padSysData *padSysGet(s32 padId)
{
    return &D_0013D9C0[padId];
}

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysReadForLoader);

INCLUDE_ASM("asm/nonmatchings/os/padSys", padSysVibSetAcrParam);

#include "padSys.h"
#include "common.h"
#include "sdk/ee/libpad2.h"
#include "sdk/ee/libvib.h"

struct t_padSysData D_0013D9C0[PAD_SYS_NR_PADS]; // 13D9C0

void padSysTickProc(void)
{
    struct t_padSysData *padsys;
    s32 i;

    for (i = 0; i < PAD_SYS_NR_PADS; i++)
    {
        padsys = &D_0013D9C0[i];
        padsys->pad2State = scePad2GetState(padsys->socket);
        if (padsys->pad2State == 1)
        {
            switch (padsys->gotProfile)
            {
            case 0:
                scePad2GetButtonProfile(padsys->socket, (unsigned char *)&padsys->buttonProfile);
                padsys->buttonData.buttonsHi = 0;
                padsys->buttonData.buttonsLo = 0;
                padsys->buttonData.analogRHor = 0x7F;
                padsys->buttonData.analogRVer = 0x7F;
                padsys->buttonData.analogLHor = 0x7F;
                padsys->buttonData.analogLVer = 0x7F;
                padsys->gotProfile++;
                break;
            case 1:
                scePad2Read(padsys->socket, &padsys->buttonData.buttonsLo);
                padsys->buttonData.buttonsLo = padsys->buttonProfile.buttonsMaskLo & ~padsys->buttonData.buttonsLo;
                padsys->buttonData.buttonsHi = padsys->buttonProfile.buttonsMaskHi & ~padsys->buttonData.buttonsHi;
                if (padsys->buttonProfile.hasAnalog == 0)
                {
                    padsys->buttonData.analogRHor = 0x7F;
                    padsys->buttonData.analogRVer = 0x7F;
                    padsys->buttonData.analogLHor = 0x7F;
                    padsys->buttonData.analogLVer = 0x7F;
                }
                break;
            }
        }
        else
        {
            padsys->gotProfile = 0;
            padsys->buttonData.buttonsHi = 0;
            padsys->buttonData.buttonsLo = 0;
            padsys->buttonData.analogRHor = 0x7F;
            padsys->buttonData.analogRVer = 0x7F;
            padsys->buttonData.analogLHor = 0x7F;
            padsys->buttonData.analogLVer = 0x7F;
        }
    }
}

void padsysInit(void)
{
    scePad2SocketParam padSocketParam;
    struct t_padSysData *temp;
    s32 i;

    scePad2Init(0);

    for (i = 0; i < PAD_SYS_NR_PADS; i++)
    {
        padSocketParam.port = i != 0;
        padSocketParam.option = 2;
        padSocketParam.slot = 0;
        temp = &D_0013D9C0[i];
        temp->socket = scePad2CreateSocket(&padSocketParam, (void *)temp);
        temp->gotProfile = 0;
    }
}

struct t_padSysData *padSysGet(s32 padId)
{
    return &D_0013D9C0[padId];
}

s32 padSysReadForLoader(void)
{
    if (scePad2Read(D_0013D9C0[0].socket, (unsigned char *)&D_0013D9C0[0].buttonData) < 0)
    {
        return 0;
    }
    return ((D_0013D9C0[0].buttonData.buttonsLo << 8) | D_0013D9C0[0].buttonData.buttonsHi) ^ 0xFFFF;
}

void padSysVibSetAcrParam(struct t_padSysData *arg0, struct t_scePad2ButtonProfile *arg1)
{
    u8 sp0[2];
    u8 sp10;

    sp0[0] = (arg1->buttonsMaskLo & 1) | ((arg1->buttonsMaskHi & 0x7F) << 1);
    sp0[1] = (arg1->buttonsMaskHi & 0x80) >> 7;
    sp10 = 3;
    sceVibSetActParam(arg0->socket, 1, &sp10, 2, sp0);
}

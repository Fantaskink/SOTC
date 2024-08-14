#include "common.h"
#include "usbSerialSys.h"

extern int semaId;
extern int iSignalSema(int sema_id);

INCLUDE_ASM("asm/nonmatchings/os/usbSerialSys", usbSerialSysPutString);

s32 usbSerialSysPrintf(char *format, ...)
{
    char buffer[0x100] __attribute__((aligned(16)));
    s32 ret;
    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);
    ret = usbSerialSysPutString(buffer);

    va_end(args);
    return ret;
}

INCLUDE_ASM("asm/nonmatchings/os/usbSerialSys", usbSerialSysInit);

static void usbSerialSysISignalSema__sub_1053F0(void)
{
    iSignalSema(semaId);
}

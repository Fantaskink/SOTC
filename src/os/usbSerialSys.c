#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifrpc.h"
#include "usbSerialSys.h"

extern s32 semaId;
extern sceSifClientData D_0013EC40;

INCLUDE_ASM("asm/nonmatchings/os/usbSerialSys", usbSerialSysPutString);

s32 usbSerialSysPrintf(char *format, ...)
{
    char buffer[0x100];
    s32 ret;
    va_list args;
    va_start(args, format);

    vsprintf(buffer, format, args);
    ret = usbSerialSysPutString(buffer);

    va_end(args);
    return ret;
}

s32 usbSerialSysInit(void) {
    unsigned int i;	
	struct SemaParam sema_p;
	
	/* Create Sema */
	sema_p.initCount = 1;
	sema_p.maxCount = 1;
	semaId = CreateSema(&sema_p);
	if (semaId == -1) {
	  printf("CreateSema Error\n");
	  while(1);
	}
	
	/* Resister RPC */
	sceSifInitRpc(0);

	while( 1 ) {
		if (sceSifBindRpc(&D_0013EC40, USB_SERIAL_BIND_ID, 0) < 0) {
			printf("bind errr\n");
			while( 1 );
		}
		if (D_0013EC40.serve != 0) break;
		i = 0x10000;
		while(i --);
	}
	
	return 0;
}

static void usbSerialSysISignalSema__sub_1053F0(void)
{
    iSignalSema(semaId);
}

// TODO: Remove me when putString's rodata is matched
IN_RODATA static char padding[] = "";

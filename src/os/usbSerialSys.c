#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifrpc.h"
#include "usbSerialSys.h"

s32 semaId;
static sceSifClientData D_0013EC40; // 0013EC40
static u32 D_0013EC80[USB_SERIAL_BUF_SEND_SZ/sizeof(u32)] __attribute__((aligned(64))); //0013EC80, sz=0x40, sent string data; Might have 0x40-align, making it not start at 0013EC70, or there is more between it and 0013EC40(0013EC68). Could also use a union for the char/u32 access but I am lazy, and this is coded semi-bad anyway.
static u32 D_0013ECC0[USB_SERIAL_BUF_RECV_SZ/sizeof(u32)] __attribute__((aligned(64))); //0013ECC0, sz=0x20, received result int

s32 usbSerialSysPutString(char *strIn) {
    s32 cLen, sLen, cct, res;
    char *cpt;
    sLen = strlen(strIn);
    
    while (sLen > 0) {
        if (sLen < USB_SERIAL_BUF_SEND_SZ) {
            strcpy((char*)D_0013EC80, strIn);
            //They thought this was char and used the sizeof as number of entries and wrote it as below, yet it should have been cast to char array before accessing. I was thinking of using CONST_ARR_NRE(D_0013EC80), but the original is wrong as below and writes in foreign mem, at 0013EC80 + 0xFC = 0013ED7C = word write of 0.
            D_0013EC80[sizeof(D_0013EC80) -1] = 0;    //Make sure it ends in 0; but why a word write?
            cLen = sLen;
        } else {
            cLen = sizeof(D_0013EC80);
            //It bravely assumes that the input buffer is aligned to doubleWord.
            memcpy(D_0013EC80, strIn, sizeof(D_0013EC80));
        }

        for (cpt = (char*)D_0013EC80, cct = 0; cct < cLen; cct++, cpt++) {
            if (*cpt == '\n') {
                    *cpt = '\r';    //I guess the PL2023 or their receiver didn't support newlines normally... or sth...
            }
        }

        WaitSema(semaId);    //Wait for any previous transfers to complete.

        res = sceSifCallRpc(&D_0013EC40, USB_SERIAL_FUNC_SEND, 0,
            D_0013EC80, sizeof(D_0013EC80),
            D_0013ECC0, sizeof(D_0013ECC0),
            usbSerialSysISignalSema, NULL);

        if (res != 0) return res;

        strIn += cLen;    //Increment to the next buf to send, from in data.
        sLen -= cLen;
    }
    return D_0013ECC0[0];    //return result
}

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

static void usbSerialSysISignalSema(void)
{
    iSignalSema(semaId);
}

// TODO: Remove me when putString's rodata is matched
IN_RODATA static char padding[] = "";

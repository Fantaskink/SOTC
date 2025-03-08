#ifndef _USBSERIALSYS_H_
#define _USBSERIALSYS_H_

#include "common.h"

int usbSerialSysPutString(char *strIn);

int usbSerialSysPrintf(char *format, ...);

int usbSerialSysInit(void);

static void usbSerialSysISignalSema(void *arg0);

// Shared with the IOP module - should really be in a common include for both, together with the transfer buffers sizes.
#define USB_SERIAL_BIND_ID 0x80000220 // REQ_ID
#define USB_SERIAL_FUNC_SEND 0x10     // RPC func ID
#define USB_SERIAL_BUF_SEND_SZ 0x40   // In bytes.
#define USB_SERIAL_BUF_RECV_SZ 0x20

#endif /* _USBSERIALSYS_H_ */

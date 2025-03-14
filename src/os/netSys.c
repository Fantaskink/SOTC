#include "netSys.h"
#include "common.h"
#include "gcc/malloc.h"
#include "gcc/stdio.h"
#include "loaderSys3.h"
#include "loaderSysFileIO.h"
#include "padSys.h"
#include "putString.h"
#include "sdk/common/netcnfif.h"
#include "sdk/ee/libmc2.h"
#include "sdk/ee/libmrpc.h"
#include "sdk/ee/libnet.h"
#include "sdk/ee/libnet/libnetdefs.h"
#include "usbSerialSys.h"

// The buffer is accessed as an offset from D_0013D180 so it mus be an struct of this form
// I'd imagine "buffer" is sceNetcnfifData_t but that struct is actually bigger
typedef struct joined_t
{
    sceSifMClientData client_data;
    s32 pad;
    u8 buffer[0x1000];
} joined_t;

static s32 func_00104090(s32 mode);
static s32 LoadSetConfiguration(sceSifMClientData *cd, u32 *net_buf, sceNetcnfifData_t *p_data, const char *fname, const char *usr_name, u32 flags);
s32 func_00104668(s32 a0, s32 a1);
int func_00104818(void);
joined_t *LoaderSysInitTCP(void);

extern int D_0013A304;
extern joined_t D_0013D180;

#define _LOAD_MODULE_ARGS(ident, argc, args)                                             \
    {                                                                                    \
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\" ident ".IRX;1", argc, args) < 0) \
            return -1;                                                                   \
        setNewIopIdentifier(ident);                                                      \
    }

#define LOAD_MODULE_ARGS(ident, args) _LOAD_MODULE_ARGS(ident, sizeof(args), args)
#define LOAD_MODULE(ident) _LOAD_MODULE_ARGS(ident, 0, NULL)

static s32 func_00104090(s32 mode)
{
    LOAD_MODULE("INET");

    LOAD_MODULE_ARGS("NETCNF", "icon=cdrom0:\\SETTING\\SYS_NET.ICO;1\0iconsys=cdrom0:\\SETTING\\ICON.SYS;1");

    LOAD_MODULE_ARGS("INETCTL", "-no_auto\0-no_decode");

    switch (mode)
    {
    case 1:
    case 2:
        LOAD_MODULE("USBD");
        LOAD_MODULE("AN986");
        break;
    case 5:
        LOAD_MODULE("USBD");
        break;
    case 3:
    case 4:
        LOAD_MODULE("DEV9");
        LOAD_MODULE("SMAP");
        break;
    case 0:
        break;
    }

    switch (mode)
    {
    case 2:
    case 4:
        LOAD_MODULE("PPP");
        LOAD_MODULE("PPPOE");
        break;
    case 5:
        LOAD_MODULE("PPP");
        LOAD_MODULE_ARGS("", "");
        break;
    case 0:
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\USBD.IRX;1", 0, NULL);
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\AN986.IRX;1", 0, NULL);
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\DEV9.IRX;1", 0, NULL);
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\SMAP.IRX;1", 0, NULL);
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\PPP.IRX;1", 0, NULL);
        LoaderSysLoadIopModule("cdrom0:\\MODULES\\PPPOE.IRX;1", 0, NULL);
        break;
    }

    LOAD_MODULE("MSIFRPC");
    LOAD_MODULE("LIBNET");
    
    if (LoaderSysLoadIopModule("cdrom0:\\MODULES2\\NETCNFIF.IRX;1", 0, NULL) < 0)
        return -1;
    setNewIopIdentifier("NETCNFIF");

    return 0;
}

static s32 LoadSetConfiguration(sceSifMClientData *cd, u32 *net_buf, sceNetcnfifData_t *p_data, const char *fname, const char *usr_name, u32 flags)
{
    sceNetcnfifArg_t if_arg;
    s32 addr;
    s32 id;
    s32 ret;

    printf("ldnet: network resource info: \"%s\"\n", fname);
    if (flags & sceLIBNETF_AUTO_LOADMODULE)
    {
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES2\\NETCNFIF.IRX;1", 0, (void *)0) < 0)
        {
            return (-1);
        }
        setNewIopIdentifier("NETCNFIF");
    }

    /* Setup Netcnfif. */
    sceNetcnfifSetup();
    /* Prepare workarea in IOP. */
    sceNetcnfifAllocWorkarea(WORKAREA_SIZE);
    {
        sceNetcnfifArg_t *parg = &if_arg;
        while (sceNetcnfifCheck())
            ;
        sceNetcnfifGetResult(parg);
        if (parg->data < 0)
        {
            return (sceLIBNETE_NG);
        }
    }
    if (flags & sceLIBNETF_DECODE)
    {
        sceNetcnfifSetFNoDecode(sceNetcnfifArg_f_no_decode_off);
    }
    else
    {
        sceNetcnfifSetFNoDecode(sceNetcnfifArg_f_no_decode_on);
    }

    /* Get setting information. */
    FlushCache(0);
    sceNetcnfifLoadEntry(fname, sceNetcnfifArg_type_net, usr_name, p_data);
    {
        sceNetcnfifArg_t *parg = &if_arg;
        while (sceNetcnfifCheck())
            ;
        sceNetcnfifGetResult(parg);
        if (parg->data < 0)
        {
            return (sceLIBNETE_NG);
        }

        /* Get a destination address of sceNetcnfifData(). */
        {
            // Bug? sceNetcnfifGetAddr doesn't take any arguments, whatever it is, we need
            // this compiling so override the declaration here
            int sceNetcnfifGetAddr();
            sceNetcnfifGetAddr(parg->data);
        }
    }
    {
        sceNetcnfifArg_t *parg = &if_arg;
        while (sceNetcnfifCheck())
            ;
        sceNetcnfifGetResult(parg);
        if (parg->data < 0)
        {
            return (sceLIBNETE_NG);
        }

        addr = if_arg.addr;

        /* Transfer sceNetcnfifData() to IOP. */
        id = sceNetcnfifSendIOP((u32)p_data, (u32)addr, (u32)sizeof(sceNetcnfifData_t));
        if (id == 0)
        {
            return (sceLIBNETE_NG);
        }
        while (sceNetcnfifDmaCheck(id))
            ;
    }

    /* Set env. */
    sceNetcnfifSetEnv(sceNetcnfifArg_type_net);
    {
        sceNetcnfifArg_t *parg = &if_arg;
        while (sceNetcnfifCheck())
            ;
        sceNetcnfifGetResult(parg);
        if (parg->data < 0)
        {
            return (sceLIBNETE_NG);
        }
    }
    addr = if_arg.addr;

    if (flags & sceLIBNETF_AUTO_UPIF)
    {
        ret = sceInetCtlSetAutoMode(cd, net_buf, !0);
    }
    else
    {
        ret = sceInetCtlSetAutoMode(cd, net_buf, 0);
    }
    if (ret < 0)
    {
        return (ret);
    }

    ret = sceLibnetSetConfiguration(cd, net_buf, (u32)addr);
    if (ret < 0)
    {
        return (ret);
    }

    return (sceLIBNETE_OK);
}

static inline s32 _load_set_conf_only(sceSifMClientData *cd, void *net_buf, const char *conf_path, const char *usr_name, s32 flags)
{
    s32 ret;
    sceNetcnfifData_t *p_data;

    p_data = memalign(0x40, sizeof(sceNetcnfifData_t));

    if (p_data == NULL)
    {
        return sceLIBNETE_INSUFFICIENT_RESOURCES;
    }

    ret = LoadSetConfiguration(cd, net_buf, p_data, conf_path, usr_name, flags);
    free(p_data);
    return ret;
}

void *func_table[] = {&LoaderSysInitTCP, &sceMc2Init, &loaderSetResetCallback, &usbSerialSysPutString};
const char *D_001320A0[] = {NULL, "Combination4", "Combination5", "Combination6", "Combination7"};
const char *D_001320B8[] = {"Combination1"};
struct sceInetAddress D_001320C0 = {0};

s32 func_00104668(s32 a0, s32 a1)
{
    const char *conf_path;
    const char *usr_name;
    u32 flags;

    // I'd like to know why the order is like this...
    flags = 0;
    if (a0 == 0)
    {
        conf_path = "cdrom0:\\SETTING\\NET.DB;1";
        usr_name = D_001320A0[a1];
        PutStringS(PUTSTR_COL_LGREEN2, "from CD default...");
    }
    else
    {
        PutStringS(PUTSTR_COL_LLBLUE3, "from memory card...");
        conf_path = "mc0:/BWNETCNF/BWNETCNF";
        usr_name = D_001320B8[a1];
        flags = 1;
    }

    printf("ldnet: up interface no auto\n");

    if (_load_set_conf_only(&D_0013D180.client_data, &D_0013D180.buffer, conf_path, usr_name, flags) < 0)
    {
        printf("load_set_conf_only() failed.\n");
        return 0;
    }

    if (get_interface_id(&D_0013D180.client_data, &D_0013D180.buffer, &D_0013A304) < 0)
    {
        printf("get_interface_id()\n");
        return 0;
    }

    sceInetCtlUpInterface(&D_0013D180.client_data, &D_0013D180.buffer, D_0013A304);

    if (wait_get_addr_only(&D_0013D180.client_data, &D_0013D180.buffer, &D_0013A304, &D_001320C0) < 0)
    {
        printf("wait_get_addr_only() failed.\n");
        return 0;
    }

    return 1;
}

// These 2 strings are present but unreferenced, so add them to this dummy function
static inline void unused(void)
{
    printf("start_thread(): CreateThread() failed.\n");
    printf("start_thread(): StartThread() failed.\n");
}

int func_00104818(void)
{
    s32 s1;
    s32 r;
    s32 ret;

    PutStringS(PUTSTR_COL_LLBLUE, "  ");

    s1 = 0;
    r = 0;
    while (s1++ < 0x1E && !(r = padSysReadForLoader()))
    {
        Sync();
        ExecBaseProc();
    }

    switch (r)
    {
    case 0x800:
        ret = 3;
        PutStringS(PUTSTR_COL_LLBLUE, "Use hdd ether port\n");
        break;
    case 0x20:
        ret = 1;
        PutStringS(PUTSTR_COL_LLBLUE, "Use usb ether port\n");
        break;
    case 0x10:
        ret = 2;
        PutStringS(PUTSTR_COL_LLBLUE, "Use usb ether port(ppp on ehter)\n");
        break;
    case 0x80:
        ret = 4;
        PutStringS(PUTSTR_COL_LLBLUE, "Use hdd ether port(ppp on ehter)\n");
        break;
    case 0x40:
        ret = 5;
        PutStringS(PUTSTR_COL_LLBLUE, "Use modem\n");
        break;
    default:
        ret = 0;
        PutStringS(PUTSTR_COL_LLBLUE, "Default startup\n");
        break;
    }
    return ret;
}

joined_t *LoaderSysInitTCP(void)
{
    static s32 D_0013A1EC = 0;
    char hostStr[0x80];
    char ipStr[0x80];
    s32 temp_v0;

    if (D_0013A1EC != 0)
    {
        return &D_0013D180;
    }

    temp_v0 = func_00104818();
    func_00104090(temp_v0);
    sceSifMInitRpc(0);
    sceLibnetInitialize(&D_0013D180.client_data, 0x800, 0x2000, 0x20);
    if (sceLibnetRegisterHandler(&D_0013D180.client_data, &D_0013D180.buffer) < 0)
    {
        printf("reg_handler() failed.\n");
        while (1)
        {
            // Infinite loop
        }
    }
    LoaderSysLoadIopModule("cdrom0:\\MODULES2\\MCMAN.IRX;1", 0, 0);
    LoaderSysLoadIopModule("cdrom0:\\MODULES2\\MCSERV.IRX;1", 0, 0);
    PutString(PUTSTR_COL_WHITE, "\n\tInitialize network environment ");

    switch (temp_v0)
    {
    case 0:
        if (func_00104668(1, temp_v0))
        {
            break;
        }
        PutStringS(PUTSTR_COL_LORANGE, "failed.\n");
        PutString(PUTSTR_COL_WHITE, "\t\tRetry by hdd ether ");
        temp_v0 = 3;
        /* fallthrough */
    default:
        if (func_00104668(0, temp_v0))
        {
            break;
        }
        PutStringS(PUTSTR_COL_LORANGE, "failed.\n");
        PutString(PUTSTR_COL_WHITE, "\tCheck your hardware environment...\n");
    }

    if (sceInetAddress2String(&D_0013D180.client_data, &D_0013D180.buffer, (char *)&ipStr, sizeof(ipStr), &D_001320C0) != 0)
    {
        printf("ldnet: invalid addr\n");
        while (1)
        {
            // Infinite loop
        }
    }

    if (sceInetAddress2Name(&D_0013D180.client_data, &D_0013D180.buffer, 0, (char *)&hostStr, sizeof(hostStr), &D_001320C0, 0, 0) != 0)
    {
        hostStr[0] = 0;
        printf("ldnet: my address: \"%s\"\n", (char *)&ipStr);
    }
    else
    {
        printf("ldnet: my address: \"%s\"[%s]\n", (char *)&hostStr, (char *)&ipStr);
    }

    PutString(PUTSTR_COL_WHITE, "done.\n\n");
    LoaderSysUnloadIopModuleByName("mcserv", 0, 0, 0);
    LoaderSysUnloadIopModuleByName("mcman", 0, 0, 0);
    PutString(PUTSTR_COL_WHITE, "\n");
    PutString(PUTSTR_COL_CYAN, "\t\t\t---------------------------------------------\n");
    if (hostStr[0] != 0)
    {
        PutString(PUTSTR_COL_CYAN, "\t\t\tWelcome to ");
        PutString(PUTSTR_COL_LGREEN3, "\"%s\"\n", (char *)&hostStr);
    }
    PutString(PUTSTR_COL_CYAN, "\t\t\t     this ps2's ip address is ");
    PutString(PUTSTR_COL_LGREEN3, "\"%s\"\n", (char *)&ipStr);
    PutString(PUTSTR_COL_CYAN, "\t\t\t---------------------------------------------\n");
    PutStringS(PUTSTR_COL_CYAN, "\n");
    D_0013A1EC = 1;

    return &D_0013D180;
}

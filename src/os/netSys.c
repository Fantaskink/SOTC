#include "netSys.h"
#include "common.h"
#include "gcc/malloc.h"

// These are likely also const char*, used for identifiers
extern const char D_0013A188[];
extern const char D_0013A190[];
extern const char D_0013A198[];
extern const char D_0013A1A0[];
extern const char D_0013A1A8[];
extern const char D_0013A1B0[];
extern const char D_0013A1B8[];
extern const char D_0013A1C0[];
extern const char D_0013A1C8[];
extern const char D_0013A1D0[];
extern const char D_0013A1D8[];
extern const char D_0013A1E0[];

// The buffer is accessed as an offset from D_0013D180 so it mus be an struct of this form
// I'd imagine "buffer" is sceNetcnfifData_t but that struct is actually bigger
typedef struct joined_t
{
    sceSifMClientData client_data;
    s32 pad;
    u8 buffer[0x1000];
} joined_t;

extern int D_0013A304;
extern char *D_001320A0[4];
extern char *D_001320B8[];
extern struct sceInetAddress D_001320C0;
extern joined_t D_0013D180;

static s32 func_00104090(s32 mode)
{
    if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\INET.IRX;1", 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A188);

    if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\NETCNF.IRX;1", 70, "icon=cdrom0:\\SETTING\\SYS_NET.ICO;1\0iconsys=cdrom0:\\SETTING\\ICON.SYS;1") < 0)
        return -1;
    setNewIopIdentifier(D_0013A190);

    if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\INETCTL.IRX;1", 20, "-no_auto\0-no_decode") < 0)
        return -1;
    setNewIopIdentifier(D_0013A198);

    switch (mode)
    {
    case 1:
    case 2:
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\USBD.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A0);
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\AN986.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A8);
        break;
    case 5:
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\USBD.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1A0);
        break;
    case 3:
    case 4:
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\DEV9.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1B0);
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\SMAP.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1B8);
        break;
    case 0:
        break;
    }

    switch (mode)
    {
    case 2:
    case 4:
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\PPP.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C0);
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\PPPOE.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C8);
        break;
    case 5:
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\PPP.IRX;1", 0, NULL) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1C0);
        if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\.IRX;1", 1, D_0013A1D0) < 0)
            return -1;
        setNewIopIdentifier(D_0013A1D0);
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

    if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\MSIFRPC.IRX;1", 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A1D8);

    if (LoaderSysLoadIopModule("cdrom0:\\MODULES\\LIBNET.IRX;1", 0, NULL) < 0)
        return -1;
    setNewIopIdentifier(D_0013A1E0);

    if (LoaderSysLoadIopModule("cdrom0:\\MODULES2\\NETCNFIF.IRX;1", 0, NULL) < 0)
        return -1;
    setNewIopIdentifier("NETCNFIF");

    return 0;
}

static s32 LoadSetConfiguration(sceSifMClientData *cd, u32 *net_buf, sceNetcnfifData_t *p_data, char *fname, char *usr_name, u32 flags)
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
        sceNetcnfifGetAddr(parg->data);
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

INCLUDE_RODATA("asm/nonmatchings/os/netSys", D_00137130);

INCLUDE_RODATA("asm/nonmatchings/os/netSys", D_00137140);

INCLUDE_RODATA("asm/nonmatchings/os/netSys", D_00137150);

INCLUDE_RODATA("asm/nonmatchings/os/netSys", D_00137160);

INCLUDE_RODATA("asm/nonmatchings/os/netSys", D_00137170);

static inline s32 load_set_conf_only(sceSifMClientData *cd, void *net_buf, char *conf_path, char* usr_name, s32 flags)
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

s32 func_00104668(s32 a0, s32 a1)
{
    char *conf_path;
    char *usr_name;
    u32 flags;

    // I'd like to know why the order is like this...
    flags = 0;
    if (a0 == 0)
    {
        conf_path = "cdrom0:\\SETTING\\NET.DB;1";
        usr_name = D_001320A0[a1];
        PutStringS(0x80FF8000, "from CD default...");
    }
    else
    {
        PutStringS(0x8080FF00, "from memory card...");
        conf_path = "mc0:/BWNETCNF/BWNETCNF";
        usr_name = D_001320B8[a1];
        flags = 1;
    }

    printf("ldnet: up interface no auto\n");

    if (load_set_conf_only(&D_0013D180.client_data, &D_0013D180.buffer, conf_path, usr_name, flags) < 0)
    {
        printf("load_set_conf_only() failed.\n");
        return 0;
    }

    if (sceLibnetWaitGetInterfaceID(&D_0013D180.client_data, &D_0013D180.buffer, &D_0013A304, 1) < 0)
    {
        printf("get_interface_id()\n");
        return 0;
    }

    sceInetCtlUpInterface(&D_0013D180.client_data, &D_0013D180.buffer, D_0013A304);

    if (sceLibnetWaitGetAddress(&D_0013D180.client_data, &D_0013D180.buffer, &D_0013A304, 1, &D_001320C0, 0) < 0)
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

INCLUDE_ASM("asm/nonmatchings/os/netSys", func_00104818);

INCLUDE_ASM("asm/nonmatchings/os/netSys", LoaderSysInitTCP);

#include "common.h"
#include "netSys.h"

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

INCLUDE_ASM("asm/nonmatchings/os/netSys", func_00104668);

INCLUDE_ASM("asm/nonmatchings/os/netSys", func_00104818);

INCLUDE_ASM("asm/nonmatchings/os/netSys", LoaderSysInitTCP);

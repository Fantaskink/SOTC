#define sceLIBNETF_USE_WAITSEMA (0x00000008)
#define sceLIBNETE_NG (-540)
#define sceLIBNETF_DECODE (0x00000001)
#define sceLIBNETF_AUTO_UPIF (0x00000002)
#define sceLIBNETF_IFC (0x00000010)
#define sceNETCNFIF_F_USE_WAITSEMA (0x00010000)
#define sceLIBNETE_OK (0)
#define NULL 0
#define sceLIBNETF_AUTO_LOADMODULE (0x00000004)

typedef void (*sceSifMEndFunc)(void *);

enum
{
    sceNetcnfifArg_f_no_decode_off,
    sceNetcnfifArg_f_no_decode_on
};

enum
{
    sceNetcnfifArg_type_net,
    sceNetcnfifArg_type_ifc,
    sceNetcnfifArg_type_dev
};

typedef struct sceNetcnfifArg
{
    int data;
    int f_no_decode;
    int type;
    int addr;
    char fname[256];
    char usr_name[256];
    char new_usr_name[256];
} sceNetcnfifArg_t;

typedef struct _sifm_rpc_data
{
    void *paddr;       /* packet address */
    unsigned int pid;  /* packet id */
    int tid;           /* thread id */
    unsigned int mode; /* call mode */
} sceSifMRpcData;

typedef struct _sifm_client_data
{
    struct _sifm_rpc_data rpcd;
    unsigned int command;
    void *buff;
    void *gp;
    sceSifMEndFunc func;
    void *para;
    void *serve;
    int sema;
    int unbind;
    int buffersize;
    int stacksize;
    int prio;
} sceSifMClientData;

typedef struct sceNetcnfifData
{
    char attach_ifc[256];
    char attach_dev[256];
    char dhcp_host_name[256];
    char address[256];
    char netmask[256];
    char gateway[256];
    char dns1_address[256];
    char dns2_address[256];
    char phone_numbers1[256];
    char phone_numbers2[256];
    char phone_numbers3[256];
    char auth_name[256];
    char auth_key[256];
    char peer_name[256];
    char vendor[256];
    char product[256];
    char chat_additional[256];
    char outside_number[256];
    char outside_delay[256];
    int ifc_type;
    int mtu;
    int ifc_idle_timeout;
    int dev_type;
    int phy_config;
    int dialing_type;
    int dev_idle_timeout;
    int p0;
    unsigned char dhcp;
    unsigned char dns1_nego;
    unsigned char dns2_nego;
    unsigned char f_auth;
    unsigned char auth;
    unsigned char pppoe;
    unsigned char prc_nego;
    unsigned char acc_nego;
    unsigned char accm_nego;
    unsigned char p1;
    unsigned char p2;
    unsigned char p3;
    int p4[5];
} sceNetcnfifData_t __attribute__((aligned(64)));

#define WORKAREA_SIZE (0x1000)

extern int printf(const char *fmt, ...);
extern int sceNetcnfifSetup(void);
extern int sceNetcnfifAllocWorkarea(int size);
extern int sceNetcnfifSync(int mode);
extern int sceNetcnfifGetResult(sceNetcnfifArg_t *result);
extern int sceNetcnfifSetFNoDecode(int f_no_decode);
extern int sceNetcnfifLoadEntryAuto(const char *fname, int type_with_flags, const char *usr_name, sceNetcnfifData_t *addr);
// extern void FlushCache(int);
extern int sceNetcnfifLoadEntry(const char *fname, int type, const char *usr_name, sceNetcnfifData_t *addr);
extern int sceNetcnfifSendIOP(unsigned int data, unsigned int addr, unsigned int size);
extern int sceNetcnfifDmaCheck(int id);
extern int sceNetcnfifSetEnv(int type);
extern int sceInetCtlSetAutoMode(sceSifMClientData *cd, void *net_buf, int f_auto);
extern int sceNetcnfifCheck(void);
extern int sceLibnetSetConfiguration(sceSifMClientData *cd, void *net_buf, unsigned int env_addr);
extern void setNewIopIdentifier(const char *identifier);

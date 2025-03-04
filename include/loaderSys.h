#ifndef LOADERSYS_H
#define LOADERSYS_H

#include "include_asm.h"
#include "common.h"
#include "fl_xfftype.h"
#include "sdk/ee/eekernel.h"
#include "gcc/string.h"

#define R_MIPS_NONE (0)
#define R_MIPS_16 (1)
#define R_MIPS_32 (2)
#define R_MIPS_REL32 (3)
#define R_MIPS_26 (4)
#define R_MIPS_HI16 (5)
#define R_MIPS_LO16 (6)

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[m"

extern void* D_00139F04; // heap pointer
#define HEAP_START D_00139F04
#define MAX_RESET_CALLBACKS 10

struct unk
{
    s32 unk0;
    s32 unk4;
};

typedef struct unk_stack_40 {
    struct t_xffEntPntHdr* unk0;
    int unk4;
    struct t_xffRelocEnt *unk8;
    void* unkC;
} unk_stack_40;
typedef int (dispose_reloc_func)(struct t_xffEntPntHdr*, struct t_xffRelocAddrEnt*, unk_stack_40*);
typedef void*(mallocAlign_func)(s32, u32);
typedef void*(mallocMaxAlign_func)(s32);
typedef int (ldrDbgPrintf_func)(const char *, ...);

s32 LoaderSysPrintf(const char *format, ...);
extern void PutString(s32, const char *, ...);
extern void PutStringS(s32, const char *, ...);
void LoaderSysSendAbort(void);
void loaderExecResetCallback(void);
s32 func_00101B88(struct t_xffEntPntHdr*, struct t_xffRelocAddrEnt *, unk_stack_40*);
void* mallocAlignMempool(s32, u32);
void* mallocAlign0x100Mempool(s32);
void func_00101AA0(void);
s32 OutputLinkerScriptFile(struct t_xffEntPntHdr*, char*, ldrDbgPrintf_func*);
void DecodeSection(void *, mallocAlign_func*, mallocMaxAlign_func*, ldrDbgPrintf_func*);
void RelocateElfInfoHeader(struct t_xffEntPntHdr* xffEp);
void initmemprintf(s32, s32);
// s32 LoaderSysFRead(s32 fd, void *buf, s32 count);
void func_001021E0(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128* gpr);
void func_00102360(u32 stat, u32 cause, u32 epc, u32 bva, u32 bpa, u128* gpr);
s32 main(s32 argc, char** argv);
void LoaderSysExecuteRecoveryFirstProcess(void);
void LoaderSysFlushPrint(void);
void LoaderSysEntryExternalThreadList(s32 thread_id);

typedef (*t_resetCallback)();
extern s32 LOADER_RESET_CALLBACK_NUM;
extern t_resetCallback RESET_CALLBACK_LIST[MAX_RESET_CALLBACKS];

#define SEMAPHORE_LIST D_0013BD10
#define THREAD_LIST D_0013B910
#define IOP_MEMORY_LIST D_0013C910
#define INTC_HANDLER_LIST D_0013C110

#define MAX_INTC_HANDLERS 256
#define IOP_MEM_LIST_LEN 256
#define MAX_IOP_IDENTIFIERS 64

extern s32 D_0013BD10[MAX_SEMAPHORES];
extern s32 D_0013B910[MAX_THREADS];
extern s32 D_0013C910[IOP_MEM_LIST_LEN];
extern struct unk D_0013C110[MAX_INTC_HANDLERS];

extern s32 D_0013A108;        // number of iop modules
extern char D_0013CD10[MAX_IOP_IDENTIFIERS][16]; // iop module identifiers

// rodata externs
extern const char D_00136200[]; // "ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n"
extern char D_00136A80[]; // "ldsys: setNewIopIdentifier: set new iop identifier \"%s\" at #%d\n", Referenced in LoaderSysLoadIopModuleFromEEBuffer

// sdata externs
extern const char D_00131DB8[]; // "XFF2"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

// Legitimate Static inlines
// TODO: This points to a custom section, so this should be a linker symbol
static inline int LoaderSysGetTextNopBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(func_00131CAC)\n" "nop\n"
        "addiu %0,%%lo(func_00131CAC)\n" "nop\n"
         : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetStackBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_stack)\n" "nop\n"
        "addiu %0,%%lo(_stack)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetStackSize() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_stack_size)\n" "nop\n"
        "addiu %0,%%lo(_stack_size)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetHeapBase() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_end)\n" "nop\n"
        "addiu %0,%%lo(_end)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

static inline int LoaderSysGetHeapSize() {
    int ret;
    asm volatile (
        "lui   %0,%%hi(_heap_size)\n" "nop\n"
        "addiu %0,%%lo(_heap_size)\n" "nop\n"
        : "=r"(ret) :
    );
    return ret;
}

// TODO: merge these
static inline s32 __inline__checkExistString(char *string, char **strings)
{
    s32 i = 0;

    while (strings[i] != 0)
    {
        if (strcmp(strings[i], string) == 0)
        {
            return 1;
        }
        i++;
    }

    return 0;
}

static inline void* __inlined_mallocAlignMempool(s32 size, u32 align) {
    void* result = (void*)((((u32)D_00139F04 + align - 1) / align) * align);
    D_00139F04 = (void*)((u32)result + size);
    return result;
}

static inline s32 __inlined_RelocateCode(struct t_xffEntPntHdr* xffEp) {
    s32 i;
    s32 j;
    
    while (xffEp != NULL) {
        struct t_xffRelocEnt *rt = xffEp->relocTab;
        for (i = xffEp->relocTabNrE; i--; rt++) {
            switch (rt->type) {
                case 4:
                case 9:
                {
                    s32 count = rt->nrEnt;
                    for (j = 0; j < count; j++) {
                        ResolveRelocation(xffEp, rt, j);
                    }
                }
                break;
            }
        }
        
        if (xffEp->nextXffHdr == 0x0) {
            xffEp = NULL;
        } else {
            printf(GSTR(D_00136200, "ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n"), ((u32)xffEp + xffEp->nextXffHdr));
            xffEp = (struct t_xffEntPntHdr*)((u32)xffEp + xffEp->nextXffHdr);
        }
    }
    
    return 1;
}

static inline s32 __inlined_RelocateSelfSymbol(struct t_xffEntPntHdr* xffEp, void* arg1) {
    s32 symtab_count = xffEp->symTabNrE;
    struct t_xffSymEnt *st = &xffEp->symTab[0];
    struct t_xffSymRelEnt *rt = &xffEp->symRelTab[0];
    struct t_xffSectEnt *stt = &xffEp->sectTab[0];
    
    for (; symtab_count--; st++, rt++) {
        u32 section = st->sect;

        switch (section) {
            case 0:
            st->addr = arg1;
            break;
            case 0xFFF1:
            st->addr = (void*)rt->offs;
            break;
            default:
            if (section <= 0xFEFF) {
                switch(st->type & 0xF) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    st->addr = (void*)(rt->offs + (s32)stt[section].memPt);
                    break;
                }
            }
            break;
        }
    }

    return 1;
}

static inline void __inlined_DisposeRelocationElement(struct t_xffEntPntHdr* xffEp, dispose_reloc_func* arg1, void* arg2) {
    s32 i;   
    s32 reloc_count;
    s32 total_relocs;

    if (xffEp->ident == *(u32*)GSTR(&D_00131DB8, "XFF2")) {
        reloc_count = xffEp->relocTabNrE >> 1;
        
        total_relocs = 0x0;
        for (i = 0; i < reloc_count; i++) {
            total_relocs += xffEp->relocTab[i].nrEnt;
        }
        
        arg1(xffEp, &xffEp->relocTab->addr[total_relocs], arg2);
         
        for (i = 0; i < reloc_count; i++) {
            xffEp->relocTab[reloc_count + i].nrEnt = 0;
        }
    }
}

static inline s32 __inlined_LoaderSysRelocateOnlineElfInfo(struct t_xffEntPntHdr* xffEp, void* arg1, void* arg2, void* arg3, void* arg4) {

    RelocateElfInfoHeader(xffEp);
    DecodeSection(xffEp, arg1, arg2, arg4);
    __inlined_RelocateSelfSymbol(xffEp, arg3);
    __inlined_RelocateCode(xffEp);
    
    return 1;
}

static inline void __inlined_LoaderSysChangeExternalThreadPriorityExceptMe(s32 priority) {
    s32 i;
    s32 threadId;

    threadId = GetThreadId();
    for (i = 0; i < MAX_THREADS; i++)
    {
        if ((THREAD_LIST[i] != threadId) && (-1 < THREAD_LIST[i]))
        {
            ChangeThreadPriority(THREAD_LIST[i], priority);
        }
    }
    return;
}

static inline void __inlined_LoaderSysDeleteAllExternalThreadExceptMe(void)
{
    s32 threadId;
    s32 i;

    threadId = GetThreadId();

    for (i = 0; i < MAX_THREADS; i++)
    {
        if ((THREAD_LIST[i] != threadId) && (THREAD_LIST[i] >= 0))
        {
            TerminateThread(THREAD_LIST[i]);
            DeleteThread(THREAD_LIST[i]);
            THREAD_LIST[i] = -1;
        }
    }
}

static inline void __inlined_LoaderSysDeleteAllExternalIopMemory(void)
{
    s32 i;

    sceSifInitRpc(0);
    sceSifInitIopHeap();

    for (i = 0; i < IOP_MEM_LIST_LEN; i++)
    {
        if (IOP_MEMORY_LIST[i] != 0)
        {
            sceSifFreeIopHeap((void *)IOP_MEMORY_LIST[i]);
            IOP_MEMORY_LIST[i] = 0;
        }
    }
}

static inline void __inlined_LoaderSysInitExternalSemaList(void)
{
    s32 i;
    for (i = 0; i < MAX_SEMAPHORES; i++)
    {
        SEMAPHORE_LIST[i] = -1;
    }
}

static inline void __inlined_LoaderSysInitExternalIntcHandlerList(void)
{
    s32 i;

    for (i = 0; i < MAX_INTC_HANDLERS; i++)
    {
        INTC_HANDLER_LIST[i].unk0 = INTC_HANDLER_LIST[i].unk4 = -1;
    }
}

static inline void __inlined_setNewIopIdentifier(const char* newIdentifier) {
    LoaderSysPrintf(D_00136A80, newIdentifier, D_0013A108);
    strncpy(D_0013CD10[D_0013A108++], newIdentifier, strlen(newIdentifier));
}

#endif /* LOADERSYS_H */

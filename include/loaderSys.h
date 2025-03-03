#ifndef LOADERSYS_H
#define LOADERSYS_H

#include "include_asm.h"
#include "common.h"
#include "fl_xfftype.h"

#define R_MIPS_NONE (0)
#define R_MIPS_16 (1)
#define R_MIPS_32 (2)
#define R_MIPS_REL32 (3)
#define R_MIPS_26 (4)
#define R_MIPS_HI16 (5)
#define R_MIPS_LO16 (6)

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_RESET "\x1b[m"

extern u32 D_00139F04; // heap pointer
#define HEAP_START D_00139F04
#define MAX_RESET_CALLBACKS 10

typedef struct unk_stack_40 {
    struct t_xffEntPntHdr* unk0;
    int unk4;
    struct t_xffRelocEnt *unk8;
    void* unkC;
} unk_stack_40;
typedef int (dispose_reloc_func)(struct t_xffEntPntHdr*, struct t_xffRelocAddrEnt*, unk_stack_40*);

// rodata externs
extern const char D_00136200[]; // "ld:\t" ANSI_BLUE "next header: %p" ANSI_RESET "\n"

// sdata externs
extern const char D_00131DB8[]; // "XFF2"
extern const char D_0013A118[]; // "\"%s\""
extern const char D_0013A120[]; // "... "
extern const char D_0013A128[]; // "Done.\n"

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

static inline s32 __inlined_mallocAlignMempool(s32 size, u32 align) {
    int result = ((D_00139F04 + align - 1) / align) * align;
    D_00139F04 = result + size;
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

#endif /* LOADERSYS_H */

#include "common.h"
#include "xfftype.h"
#include "fl_xfftype.h"

extern void* memcpy(void* dest, const void* src, u64 n);
extern void* memset(void* s, s32 c, u64 n);
extern char* D_00131DC0[]; // Array of strings
extern char D_00136318[]; // "ld:\t\tdecode section\n"
extern char D_00136330[]; // "ld:\t%15s(progbit): 0x%08x(0x%08x) %s\n"
extern char D_00136358[]; // "ld:\t%15s(overlaydata): 0x%08x(0x%08x) %s\n"
extern char D_00136388[]; // "ld:\t%15s(nobit)  : 0x%08x(0x%08x) %s\n"

INCLUDE_ASM(const s32, "os/loadersys1", ResolveRelocation);

void DecodeSection(
    void* xffBuf,
    void* (*mallocAlign)(int sz, int align),
    void* (*mallocMaxAlign)(int sz),
    void (*ldrDbgPrintf)(char* fmt, ...)
) {
    int i;
    struct t_xffEntPntHdr* xffEp;
    void* entPntSectBs = NULL;
    struct t_xffSsNmOffs* nmOffs;
    struct t_xffSectEnt* sect;

    xffEp = xffBuf;
    
    i = xffEp->sectNrE-1;
    sect = &xffEp->sectTab[1];
    nmOffs = &xffEp->ssNamesOffs[0];
    
    if (ldrDbgPrintf != NULL)
        ldrDbgPrintf(D_00136318);
    
    // The zero section is not processed as it is all 0.
    for (; i--; sect++, nmOffs++) {
        
        sect->moved = 0;
        if (sect->size != 0) {
            switch (sect->type) {
                case 1:
                case 0x7FFFF420: // .text section
                    if (sect->flags != 0) {
                        // Forced max alignment
                        sect->memPt = mallocMaxAlign(sect->size);
                        memcpy((void*)sect->memPt, (void*)sect->filePt, sect->size);
                        sect->moved = 2;
                    } else {
                        // Check if the alignment is enough as is in the file:
                        if (((u32)sect->filePt & (sect->align - 1)) != 0) {
                            // Insufficient alignment, so alloicate
                            sect->memPt = mallocAlign(sect->size, sect->align);
                            memcpy((void*)sect->memPt, (void*)sect->filePt, sect->size);
                            sect->moved = 1;
                        } else { // use the section as is in the file
                            sect->memPt = sect->filePt;
                        }
                    }
                    if (ldrDbgPrintf != NULL) {
                        if (sect->type == 1) { // .text
                            // The moved-types are actually allocation types (or used in file as is).
                            ldrDbgPrintf(D_00136330, &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                        } else { // VU code/data
                            // The moved-types are actually allocation types (or used in file as is).
                            ldrDbgPrintf(D_00136358, &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                        }
                    }
                break;
                case 8: // nobit .bss section
                    // Because nobits is not present in the file, it is always allocated
                    if (sect->flags != 0) { // section is not copied and used as is in the file
                        sect->memPt = mallocMaxAlign(sect->size);
                        sect->moved = 2;
                    } else {
                        sect->memPt = mallocAlign(sect->size, sect->align);
                        sect->moved = 1;
                    }
                    
                    memset(sect->memPt, 0x00, (u64)sect->size);
                    if (ldrDbgPrintf != NULL) {
                        // The moved-types are actually allocation types (or used in file as is).
                        ldrDbgPrintf(D_00136388, &xffEp->ssNamesBase[nmOffs->nmOffs], sect->memPt, sect->size, D_00131DC0[sect->moved]);
                    }
                break;
            }
        } else {
            sect->memPt = NULL;
        }
        
        if (entPntSectBs == NULL)
            entPntSectBs = sect->memPt;
    }

    xffEp->entryPnt = (void*)((u32)entPntSectBs + xffEp->entryPnt_Rel);
    return;
}

INCLUDE_ASM(const s32, "os/loadersys1", RelocateElfInfoHeader);

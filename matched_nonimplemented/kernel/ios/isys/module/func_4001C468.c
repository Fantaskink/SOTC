#include "fl_xfftype"

void iosFree(void*);

struct t_xffEntPntHdr* func_4001C468(struct t_xffEntPntHdr* xff) {
    s32 i;
    struct t_xffSectEnt *cSec;

    if (xff != NULL) {
        cSec = xff->sectTab + 1;
        for(i=xff->sectNrE-2; i!=-1; i--){
            if (cSec->moved != 0) {
                cSec->moved = 0;
                iosFree(cSec->memPt);
            }
            cSec++;
        }
        iosFree(xff);
    }
    return xff;
}


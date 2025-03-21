#include "sdk/libcdvd.h"

extern int strncmp(const char* str1, const char* str2, s32 num);
extern s32 LoaderSysFOpen(const char *name, s32 flags, s32 mode);
extern void iosJumpRecoverPoint(const char* msg, ...);
extern char* strcpy(char* dest, const char* src);


extern char D_40046460;
extern char D_40046468;
extern s32 D_40049A7C;
extern s32 D_40049A80;
extern s32 D_40049A84;
extern s64 D_40051AA8;
extern s64 D_40051AB0;
extern s64 D_40051AB8;
extern char D_40051AC0;



void iosCdStInit(char* filename)
{
    sceCdlFILE file;
    
    D_40049A80 = strncmp(&D_40046460, filename, 6) == 0;
    
    D_40051AB8 = D_40051AA8 = D_40051AB0 = -1;
    
    if (!D_40049A80)
    {
        if (sceCdSearchFile(&file, filename) == 0)
            asm("break");
        
        D_40049A7C = file.lsn;
        strcpy(&D_40051AC0, filename);
        return;
    }
    
    D_40049A84 = LoaderSysFOpen(filename, 1, 0x1FF);
    
    if (D_40049A84 < 0)
        iosJumpRecoverPoint(&D_40046468, filename);
}


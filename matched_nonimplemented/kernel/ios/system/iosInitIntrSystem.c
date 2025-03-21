struct SemaParam {
    s32 currentCount;
    s32 maxCount;
    s32 initCount;
    s32 numWaitThreads;
    u32 attr;
    u32 option;
};
extern int CreateSema( struct SemaParam *param );
extern void LoaderSysEntryExternalSemaList(s32 arg1);
extern void iosAddIntcHandler(s32 arg0, void* arg1);
extern void D_40013058();
extern s32 D_40052B10;
extern s32 D_40052B14;
extern s32 iosSystemSema;

#define T0_COUNT        ((volatile u32 *)(0x10000000))
#define T0_MODE         ((volatile u32 *)(0x10000010))
#define T1_COUNT        ((volatile u32 *)(0x10000800))
#define T1_MODE         ((volatile u32 *)(0x10000810))


void iosInitIntrSystem(void)
{
    s32 sema;
    struct SemaParam semaParam;

    *T0_MODE = 0;
    *T1_MODE = 0;
    iosAddIntcHandler(9, D_40013058);
    *T0_COUNT = 0;
    *T1_COUNT = 0;
    *T0_MODE = 0x280;
    *T1_MODE = 0x80;
    
    semaParam.initCount = 1;
    semaParam.maxCount = 1;
    sema = CreateSema(&semaParam);
    LoaderSysEntryExternalSemaList(sema);
    iosSystemSema = sema;
    D_40052B10 = -1;
    D_40052B14 = 0;
}


extern s32 D_40045544;

#define T1_COUNT        ((volatile u32 *)(0x10000800))
#define T1_MODE         ((volatile u32 *)(0x10000810))

void iosResetCpuRapCounter(u32 arg0)
{
    D_40045544 = arg0;
    *T1_MODE = arg0 | 0x80;
    *T1_COUNT = 0;
}


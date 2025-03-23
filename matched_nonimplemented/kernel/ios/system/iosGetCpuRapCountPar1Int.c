extern s32 D_40045544;
extern f32 D_40045548[];

#define T1_COUNT        ((volatile u32 *)(0x10000800))

f32 iosGetCpuRapCountPar1Int(void)
{
    f32 var_f0;
    
    var_f0 = (f32)*T1_COUNT;
    return (var_f0 * 60.0f) / D_40045548[D_40045544];
}


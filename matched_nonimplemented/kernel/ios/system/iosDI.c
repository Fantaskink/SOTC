extern s32 DIntr();
extern void iosJumpRecoverPoint(const char* msg, ...);

extern s32 D_40045554;
extern char D_40046518;
extern char D_40046530;

void iosDI(void)
{
    if (DIntr() != 0)
    {
        if (D_40045554 != 0)
            iosJumpRecoverPoint(&D_40046518, D_40045554);
        
    }
    else if (D_40045554 <= 0)
        iosJumpRecoverPoint(&D_40046530, D_40045554);
    
    D_40045554 += 1;
}


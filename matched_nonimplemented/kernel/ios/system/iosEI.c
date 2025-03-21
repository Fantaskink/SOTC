extern s32 EIntr(); 
extern void iosJumpRecoverPoint(const char* msg, ...);

extern s32 D_40045554;
extern char D_40046548;
extern char D_40046578;

s32 iosEI(void)
{
    D_40045554--;
    if (D_40045554 == 0)
    {
        if (EIntr() != 0)
            iosJumpRecoverPoint(&D_40046548);
        
    }
    else if (D_40045554 < 0)
        iosJumpRecoverPoint(&D_40046578);
    
    return D_40045554;
}


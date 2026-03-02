extern s32 D_40045A00;

extern void iosDlChainTailCurrent(s32);

s32 iosSetCurrentDC()

{
    iosDlChainTailCurrent(D_40045A00);
    return;
}
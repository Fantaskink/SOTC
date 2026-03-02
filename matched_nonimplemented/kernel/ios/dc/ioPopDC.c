
extern s32 D_40045A08; //gLastindex
extern s32 D_40092284[]; //indextotop
extern s32 D_40045A00; //gdata

extern void iosDlChainTailCurrent(s32 index);
void iosPopDC() 
{
    s32 indexHelper;
    
    D_40045A08--;
        indexHelper = D_40092284[D_40045A08];
    iosDlChainTailCurrent(indexHelper);
    D_40045A00 = indexHelper;
    return;
}
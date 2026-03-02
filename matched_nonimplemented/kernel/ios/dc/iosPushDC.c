
extern s32 D_40045A08; //glastindex
extern s32 D_40092288;//gbased index
extern s32 D_40045A00; //gdata

extern void iosDlChainTailCurrent(s32 data);
void iosPushDC(int currentData) {
    s32 *indexHelper = &D_40092288 + D_40045A08;
    ++D_40045A08;
    *indexHelper = currentData;
    iosDlChainTailCurrent(currentData);
    D_40045A00 = currentData;
    return;
}

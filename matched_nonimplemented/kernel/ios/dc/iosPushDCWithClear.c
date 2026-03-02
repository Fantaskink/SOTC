extern s32 D_40045A08; //glastindex
extern s32 D_40092288[];  //gbasedindex
extern s32 D_40045A00; //gdata

extern void iosDlChainTailCurrent(s32* index);

void iosPushDCWithClear(s32* param_1) 
{
    s32 *indexHelper;

    indexHelper = D_40092288 + D_40045A08;
    D_40045A08++;
    *indexHelper = param_1;
    iosDlChainTailCurrent(param_1);
    if ( ( param_1[92] & 1) != 0) {
      iosDlChainTailCurrent(param_1 + 52);
    }
    D_40045A00 = param_1;
    return;
}
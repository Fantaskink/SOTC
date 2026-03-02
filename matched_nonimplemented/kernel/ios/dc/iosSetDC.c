extern void iosDlChainTailCurrent(s32 *currentData);
s32 iosSetDC(s32 *param_1)
{
    iosDlChainTailCurrent(param_1);
    if ( ( param_1[92] & 1) != 0) {
        iosDlChainTailCurrent( param_1+52 );
        return;
    }
    return;
}


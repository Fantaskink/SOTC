extern volatile s64 D_40052B08;

#define T0_COUNT        ((volatile u32 *)(0x10000000))

s64 iosGetTCountExtension(void)
{
    return D_40052B08 | *T0_COUNT;
}


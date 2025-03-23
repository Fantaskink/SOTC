extern s64 D_40052B08;

s64 iosGetTCount(void)
{
    volatile s64* tmp = &D_40052B08;
    return *tmp;
}


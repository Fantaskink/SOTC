extern s32 D_40049A74;
extern s32 D_40049A78;
extern s32 D_40049A7C;
extern s64 D_40051AB8;

void iosCdStStart(s32 arg0)
{
    D_40049A74 = D_40049A78;
    D_40051AB8 = (s64)(arg0 + D_40049A7C) << 11;
}


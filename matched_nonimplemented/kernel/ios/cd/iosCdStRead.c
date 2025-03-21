extern void func_400121D0(u32 lsn);

extern void D_40049AA8;
extern s64 D_40051AA8;
extern s64 D_40051AB0;
extern s64 D_40051AB8;


void iosCdStRead(void* buf, s32 len)
{
    s32 count;
    s64 tmp0;

    while(len > 0)
    {
        if(D_40051AB8 >= D_40051AA8 && D_40051AB0 > D_40051AB8)
        {
            count = D_40051AB0 - D_40051AB8;
            count = (len <= count) ? len : count;
            
            memcpy(buf, &D_40049AA8 + (D_40051AB8 - D_40051AA8), count);
            len -= count;
            buf += count;
            D_40051AB8 += count;
        }
            
        else
        {
            tmp0 = D_40051AB8 < 0 ? D_40051AB8 + 2047 : D_40051AB8;
            func_400121D0(tmp0 >> 11);
        }
    }
}


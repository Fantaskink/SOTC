void *memset(void *, int, unsigned long); /* extern */

typedef struct
{
    int unk0;
    int unk4;
} unk_struct;

void iosCreateMsgQueue(unk_struct *arg0, int arg1, int arg2)
{
    memset(arg0, 0, 0x1C);
    if ((arg1 != 0) && (arg2 > 0))
    {
        memset(arg1, 0, arg2 * 8);
    }
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
}
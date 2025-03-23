typedef struct
{
    char padding[0x8];
    long unk8;
} unk_struct;

void sndSepCont(unk_struct *arg0)
{
    arg0->unk8 &= ((((-3LL << 16) | 0xFFFF) << 16) | 0xFFFF);
}

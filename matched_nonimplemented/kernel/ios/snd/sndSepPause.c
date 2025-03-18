typedef struct
{
    char padding[0x8];
    long unk8;
} unk_struct;

void sndSepPause(unk_struct *arg0)
{
    arg0->unk8 |= (0x8000LL << 18);
}

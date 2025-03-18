typedef struct
{
    char padding[0x20];
    int unk20;
} unk_struct;

void init_letter(unk_struct *arg0, int *arg1, int *arg2)
{
    *arg1 = 0;
    *arg2 = 0;
    arg0->unk20 = 1;
}

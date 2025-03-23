typedef struct
{
    char padding[0xc];
    int unkC;
} unk_struct;

int is_hope_letter_type(unk_struct *arg0, int arg1)
{
    return arg0->unkC == arg1;
}

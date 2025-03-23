extern u32 D_40049A68;
extern u32 D_40049A70;
extern u32* D_40049A78;

void iosCdSetMode(s32 mode)
{
    D_40049A78 = (mode == 1) ? &D_40049A68 : &D_40049A70;
}


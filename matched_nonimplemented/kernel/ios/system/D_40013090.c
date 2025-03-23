extern void (*D_40052B18[])();

void D_40013090(void)
{
    s32 i;

    for (i=0; i<2; i++)
    {
        if (D_40052B18[i] != NULL)
            D_40052B18[i]();
    }
}


extern int D_400E7650;

int Sg2SlotSetOutputMode(int arg0)
{
    int temp_v0;

    temp_v0 = D_400E7650;
    D_400E7650 = arg0;
    return temp_v0;
}

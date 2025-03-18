int GetStrHashWorkBuffer(int shift, int index)
{
    return (4 << shift) + (index * 0x10) + 0x66;
}

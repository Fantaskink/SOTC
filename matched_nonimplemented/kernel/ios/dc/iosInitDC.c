
extern s32 D_40092688[];

extern isysInitGroup(s32* dataOrMemory);
void iosInitDC()
{
    isysInitGroup(D_40092688); 
    //int D_40092688[2]; seem head and tail 
}
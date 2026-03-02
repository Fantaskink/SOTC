extern s32 D_40092688[];

extern void _isysDeleteGroup(s32 , s32* , s32);
extern void isysRequestDeleteObj(s32);

s32 iosReleaseDC(s32 param_1) 
{
    
    if (param_1 != 0) {
        _isysDeleteGroup(param_1,D_40092688,0);
        isysRequestDeleteObj(param_1);
        return;
    }
    return;
}
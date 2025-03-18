int GetThreadId();
extern int D_40054C08[256];

int iosGetThread(void)
{
    return D_40054C08[GetThreadId()];
}

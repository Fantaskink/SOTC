struct SemaParam
{
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};

int CreateSema(struct SemaParam *);
void LoaderSysEntryExternalSemaList(int arg1);

int iosCreateSema(int arg0, int arg1)
{
    struct SemaParam sparam;

    int temp_v0;

    sparam.initCount = arg0;
    sparam.maxCount = arg1;

    temp_v0 = CreateSema(&sparam);
    LoaderSysEntryExternalSemaList(temp_v0);
    return temp_v0;
}

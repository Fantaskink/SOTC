struct ThreadParam
{
    int status;
    void (*entry)(void *);
    void *stack;
    int stackSize;
    void *gpReg;
    int initPriority;
    int currentPriority;
    unsigned int attr;
    unsigned int option;
    int waitType;
    int waitId;
    int wakeupCount;
};

#define NULL ((void *)0)

int GetThreadId(); /* extern */
extern struct ThreadParam *D_40054C08[256];

int iosGetThreadPriority(struct ThreadParam *thread)
{
    if (thread == NULL)
    {
        thread = D_40054C08[GetThreadId()];
    }
    thread->initPriority = 0;
    return 0;
}

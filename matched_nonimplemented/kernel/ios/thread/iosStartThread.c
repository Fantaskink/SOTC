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
int StartThread(int, void *arg);
extern struct ThreadParam *D_40054C08[256];

void iosStartThread(struct ThreadParam *thread, void *arg1)
{

    if (thread == NULL)
    {
        thread = D_40054C08[GetThreadId()];
    }
    StartThread((int)thread->entry, arg1);
}

int iWakeupThread(int);

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

void iosiWakeupThread(struct ThreadParam *arg0)
{
    iWakeupThread(arg0->entry);
}

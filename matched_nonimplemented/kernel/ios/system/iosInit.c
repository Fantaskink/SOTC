extern void iosCreateIdleThread();
extern void iosCreateIopMgr();
extern void iosCreatePrintMgr();
extern void iosInitMessageSystem();
extern void iosInitThreadSystem();

void iosInit(void)
{
    iosInitThreadSystem();
    iosInitMessageSystem();
    iosCreateIdleThread();
    iosCreatePrintMgr();
    iosCreateIopMgr();
}


int iosSendMsg(int *, int, int); /* extern */
extern int D_40051AD8;

void iosWaitIdleThread(void)
{
    iosSendMsg(&D_40051AD8, 0, 1);
}

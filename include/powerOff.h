#ifndef POWEROFF_H
#define POWEROFF_H

typedef void(cbFunc_t)(void);

void LoaderSysSetPowerOffCallBackFunc(cbFunc_t *arg0);
void PreparePowerOff(void);
void PowerOffThread(void *arg);

#endif /* POWEROFF_H */
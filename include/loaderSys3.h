#ifndef LOADERSYS3_H
#define LOADERSYS3_H

#include "common.h"

typedef void (*t_resetCallback)();

void func_001033B0();
void loaderLoop(void);
s32 main(s32 argc, char **argv);
char *LoaderSysGetBootArg(void);
s32 LoaderSysLoadIopModule(const char *path, s32 arg_count, void *args);
s32 LoaderSysUnloadIopModuleByName(const char *arg0, s32 arg1, s32 arg2, s32 *arg3);
void LoaderSysHookPoint(void);
void LoaderSysRebootIop(const char *arg0);
void loaderExecResetCallback(void);
void loaderSetResetCallback(t_resetCallback callback);
s32 memprintf(const char *in, ...);
s32 imemprintf(const char *in, ...);
void initmemprintf(s32 a, s32 b);

#endif /* LOADERSYS3_H */

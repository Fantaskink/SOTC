#ifndef LOADERSYSMEMPRINT_H
#define LOADERSYSMEMPRINT_H

#include "common.h"

void initmemprintf(s32 a, s32 b);
s32 memprintf(const char *in, ...);
s32 imemprintf(const char *in, ...);

#endif /* LOADERSYSMEMPRINT_H */

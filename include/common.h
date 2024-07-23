#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

typedef char s8;
typedef short s16;
typedef int s32;
typedef long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef float f32;

#define UNK_TYPE s32
#define UNK_PTR void *
#define UNK_RET void
#define UNK_FUN_ARG void (*)(void)
#define UNK_FUN_PTR(name) void (*name)(void)
#define UNK_ARGS

#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif /* COMMON_H */
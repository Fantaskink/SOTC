#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

typedef char s8;
typedef short s16;
typedef int s32;
typedef long s64;
typedef long s128  __attribute__((mode(TI)));

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef unsigned long u128  __attribute__((mode(TI)));

typedef unsigned int u_long128 __attribute__((mode(TI)));

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

#define IN_RODATA __attribute__((section(".rodata")))
#define IN_DATA __attribute__((section(".data")))
#define IN_SDATA __attribute__((section(".sdata")))
#define IN_SBSS __attribute__((section(".sbss")))
#define IN_BSS __attribute__((section(".bss")))

#define CONST_ARR_NRE(a) ((const)(sizeof((a)) / sizeof(*(a))))

#define CONST_ARR_ENTSZ(a) (sizeof(*(a)))

#define ALIGN(v, a) (((v) + (a - 1)) & ~(a - 1))

#define GSTR_USE_PTR_REF

#ifdef GSTR_USE_PTR_REF
#define GSTR(ptr, str) ((ptr))
#else
#define GSTR(ptr, str) ((str))
#endif

#endif /* COMMON_H */

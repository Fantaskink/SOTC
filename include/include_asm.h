#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

#if !defined(SPLAT) && !defined(M2CTX) && !defined(PERMUTER)
#ifndef INCLUDE_ASM
#define INCLUDE_ASM_TOP(FOLDER, NAME)                 \
    __asm__(                                          \
        ".section .text\n"                            \
        "    .set noat\n"                             \
        "    .set noreorder\n"                        \
        "    .globl " #NAME ".NON_MATCHING\n"         \
        "    .type " #NAME ".NON_MATCHING, @object\n" \
        "    " #NAME ".NON_MATCHING:\n"

#ifndef PREPROCESS_ASM
#define INCLUDE_ASM_MIDDLE(FOLDER, NAME) \
    "    .include \"" FOLDER "/" #NAME ".s\"\n"
#else
#define INCLUDE_ASM_MIDDLE(FOLDER, NAME) \
    "    #include \"" FOLDER "/" #NAME ".s\"\n"
#endif

#define INCLUDE_ASM_BOTTOM() \
        "    .set reorder\n"                          \
        "    .set at\n")

#define INCLUDE_ASM(FOLDER, NAME)    \
    INCLUDE_ASM_TOP(FOLDER, NAME)    \
    INCLUDE_ASM_MIDDLE(FOLDER, NAME) \
    INCLUDE_ASM_BOTTOM()

#ifdef PREPROCESS_ASM
#include "asm_regs.h"
#endif

#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)                \
    __asm__(                                        \
        ".section .rodata\n"                        \
        "    .include \"" FOLDER "/" #NAME ".s\"\n" \
        ".section .text")
#endif
__asm__(".include \"include/labels.inc\"\n");
#else
#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)
#endif
#endif

#endif

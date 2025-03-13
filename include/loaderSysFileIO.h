#ifndef LOADERSYSFILEIO_H
#define LOADERSYSFILEIO_H

#include "common.h"
#include "loaderSys.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/sifdev.h"

typedef struct memory_info
{
    void *stack_base;
    void *stack_end;
    void *heap_base;
    void *heap_end;
    void *module_stack_base;
    void *module_stack_end;
    void *unk18;
    void *stack_base2;
    unk_00131D00_s module_info;
} memory_info;

s32 LoaderSysPrintf(const char *format, ...);
s32 LoaderSysCheckCDBootMode();
void LoaderSysPutString(char *string);
s32 LoaderSysFOpen(const char *name, s32 flags, s32 mode);
s32 LoaderSysFClose(s32 fd);
s32 LoaderSysFSeek(s32 fd, s32 offset, s32 whence);
s64 LoaderSysFSeek64(s32 fd, s64 offset, s32 whence);
s32 LoaderSysFRead(s32 fd, void *buf, s32 count);
s32 LoaderSysFWrite(s32 fd, const void *buf, s32 count);
s32 LoaderSysRemove(const char *name);
s32 LoaderSysRmdir(const char *name);
s32 LoaderSysMkdir(u8 *name, s32 mode);
s32 LoaderSysGetstat(const char *name, struct sce_stat *buf);
s32 LoaderSysChstat(const char *name, struct sce_stat *buf, u32 cbit);
s32 LoaderSysLoadIopModuleFromEEBuffer(const char *module_ident, s32 data, s32 module_size, s32 argc, const char *argp);
s32 LoaderSysGetMemoryInfo(memory_info *info);
void LoaderSysDumpIopModuleIdentifiers(void);
void LoaderSysSendAbort(void);
void LoaderSysFlushPrint(void);
char *checkHookDesc(char *hook_desc);
void setNewIopIdentifier(const char *newIdentifier);

static inline s32 LoaderSysGetStackBase()
{
    s32 ret;
    asm volatile(
        "lui   %0,%%hi(_stack)\n"
        "nop\n"
        "addiu %0,%%lo(_stack)\n"
        "nop\n"
        : "=r"(ret) :);
    return ret;
}

static inline s32 LoaderSysGetStackSize()
{
    s32 ret;
    asm volatile(
        "lui   %0,%%hi(_stack_size)\n"
        "nop\n"
        "addiu %0,%%lo(_stack_size)\n"
        "nop\n"
        : "=r"(ret) :);
    return ret;
}

static inline s32 LoaderSysGetHeapBase()
{
    s32 ret;
    asm volatile(
        "lui   %0,%%hi(_end)\n"
        "nop\n"
        "addiu %0,%%lo(_end)\n"
        "nop\n"
        : "=r"(ret) :);
    return ret;
}

static inline s32 LoaderSysGetHeapSize()
{
    s32 ret;
    asm volatile(
        "lui   %0,%%hi(_heap_size)\n"
        "nop\n"
        "addiu %0,%%lo(_heap_size)\n"
        "nop\n"
        : "=r"(ret) :);
    return ret;
}

#endif /* LOADERSYSFILEIO_H */

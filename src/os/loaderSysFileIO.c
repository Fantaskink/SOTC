#include "loaderSysFileIO.h"
#include "common.h"
#include "gcc/stdarg.h"
#include "gcc/stdio.h"
#include "sdk/ee/sif.h"
#include "sdk/ee/sifdev.h"
#include "sdk/ee/sifrpc.h"

#define MAX_IOP_IDENTIFIERS 64

extern struct unk_00131D00_s D_00131D00;
extern s32 D_0013A110;
extern s32 D_0013A114;
extern s32 D_00139F00;

s32 LoaderSysPrintf(const char *format, ...)
{
    char buffer[256];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    printf(buffer);

    return strlen(buffer);
}

void LoaderSysPutString(char *string)
{
    printf("%s", string);
}

void LoaderSysFlushPrint(void)
{
}

char *checkHookDesc(char *hook_desc)
{
    s32 hasDiff;
    char *desc;

    hasDiff = strncmp(hook_desc, "host0:", 6);
    desc = hook_desc + 6;
    if (hasDiff == 0)
    {
        return desc;
    }
    return NULL;
}

s32 D_0013A108 = 0;                              // number of iop modules
extern char D_0013CD10[MAX_IOP_IDENTIFIERS][16]; // iop module identifiers

static inline s32 LoaderSysSearchInLoadedIopModules(const char *module_name)
{
    s32 i;
    for (i = 0; i < D_0013A108; i++)
    {
        if (!strcmp((const char *)&D_0013CD10[i], module_name))
        {
            return i;
        }
    }
    return -1;
}

static inline s32 _loadIopModuleFromEEBuffer(s32 data, s32 module_size, s32 argc, const char *argp)
{
    s32 stat;
    void *module_addr;
    s32 queue_id;
    sceSifDmaData dma_data;

    module_size = ALIGN(module_size, 4);

    sceSifInitRpc(0x0);
    sceSifInitIopHeap();

    module_addr = sceSifAllocSysMemory(0x1, module_size, NULL);
    LoaderSysPrintf("ldsys: _loadIopModuleFromEEBuffer: allocated iop memory:%p(size:%d)\n", module_addr, module_size);
    FlushCache(0x0);
    dma_data.addr = (int)module_addr;
    dma_data.mode = 0x0;
    dma_data.data = data;
    dma_data.size = module_size;
    queue_id = sceSifSetDma(&dma_data, 0x1);

    while (-0x1 < sceSifDmaStat(queue_id))
        ;

    LoaderSysPrintf("ldsys: _loadIopModuleFromEEBuffer: senddma: finish\n");
    stat = sceSifLoadModuleBuffer(module_addr, argc, argp);
    sceSifFreeSysMemory(module_addr);
    return stat;
}

void setNewIopIdentifier(const char *newIdentifier)
{
    LoaderSysPrintf("ldsys: setNewIopIdentifier: set new iop identifier \"%s\" at #%d\n", newIdentifier, D_0013A108);
    strncpy(D_0013CD10[D_0013A108++], newIdentifier, strlen(newIdentifier));
}

s32 LoaderSysLoadIopModuleFromEEBuffer(const char *module_ident, s32 data, s32 module_size, s32 argc, const char *argp)
{
    s32 stat;
    s32 module_id;

    if (module_ident != 0x0)
    {
        module_id = LoaderSysSearchInLoadedIopModules(module_ident);
        if (module_id < 0x0)
        {
            stat = _loadIopModuleFromEEBuffer(data, module_size, argc, argp);
            if (-0x1 < stat)
            {
                setNewIopIdentifier(module_ident);
            }
        }
        else
        {
            LoaderSysPrintf("ldsys: LoaderSysLoadIopModuleFromEEBuffer: iop identifier \"%s\" exists at #%d (load skipped).\n", module_ident, module_id);
            return 0x0;
        }
    }
    else
    {
        stat = _loadIopModuleFromEEBuffer(data, module_size, argc, argp);
    }
    return stat;
}

void LoaderSysDumpIopModuleIdentifiers(void)
{
    s32 i;

    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: dump list of iop identifiers\n");
    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: ----------------------------\n");

    for (i = 0; i < D_0013A108; i++)
    {
        LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: \t% 2d: \"%s\"\n", i, D_0013CD10[i]);
    }

    LoaderSysPrintf("ldsys: LoaderSysDumpIopModuleIdentifiers: ----------------------------\n");
}

s32 LoaderSysGetMemoryInfo(memory_info *info)
{
    u32 stack_size;
    u32 heap_size;

    info->unk18 = (void *)D_00139F00;

    info->stack_base = (void *)LoaderSysGetStackBase();
    stack_size = LoaderSysGetStackSize();
    info->heap_base = (void *)LoaderSysGetHeapBase();
    heap_size = LoaderSysGetHeapSize();

    info->module_stack_base = info->module_stack_end = (void *)D_0013A114;
    info->module_stack_end += D_00131D00.stack_size;
    info->stack_base2 = info->stack_base;
    info->stack_end = info->stack_base + stack_size;
    info->heap_end = info->heap_base + heap_size;
    info->module_info = D_00131D00;

    return 1;
}

s32 LoaderSysCheckCDBootMode()
{
    return 2;
}

void LoaderSysSendAbort(void)
{
}

s32 LoaderSysFOpen(const char *name, s32 flags, s32 mode)
{
    s32 success = sceOpen(name, flags, mode);
    return success;
}

s32 LoaderSysFClose(s32 fd)
{
    s32 success = sceClose(fd);
    return success;
}

s32 LoaderSysFSeek(s32 fd, s32 offset, s32 whence)
{
    s32 success = sceLseek(fd, offset, whence);
    return success;
}

s64 LoaderSysFSeek64(s32 fd, s64 offset, s32 whence)
{
    s64 success = sceLseek64(fd, offset, whence);
    return success;
}

s32 LoaderSysFRead(s32 fd, void *buf, s32 count)
{
    s32 success = sceRead(fd, buf, count);
    return success;
}

s32 LoaderSysFWrite(s32 fd, const void *buf, s32 count)
{
    s32 success = sceWrite(fd, buf, count);
    return success;
}

s32 LoaderSysRemove(const char *name)
{
    s32 success = sceRemove(name);
    return success;
}

s32 LoaderSysRmdir(const char *name)
{
    s32 success = sceRmdir(name);
    return success;
}

s32 LoaderSysMkdir(const char *name, s32 mode)
{
    s32 success = sceMkdir(name, mode);
    return success;
}

s32 LoaderSysGetstat(const char *name, struct sce_stat *buf)
{
    return sceGetstat(name, buf);
}

s32 LoaderSysChstat(const char *name, struct sce_stat *buf, u32 cbit)
{
    return sceChstat(name, buf, cbit);
}

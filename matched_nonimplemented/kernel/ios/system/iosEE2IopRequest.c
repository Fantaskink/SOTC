extern void FlushCache(int);
extern void iosSignalIopSema();
extern void iosWaitIopSema();
extern int sceSifDmaStat(unsigned int id);

typedef struct {
	u32 data;
	u32 addr;
	u32 size;
	u32 mode;
} sceSifDmaData;
extern unsigned int sceSifSetDma(sceSifDmaData *sdd, int len);

void iosEE2IopRequest(s32 data, s32 addr, s32 size)
{
    s32 temp;
    sceSifDmaData sifDmaData;

    sifDmaData = (sceSifDmaData)
    {
        .data = data,
        .addr = addr,
        .size = size,
        .mode = 0
    };
    
    FlushCache(0);
    iosWaitIopSema();
    temp = sceSifSetDma(&sifDmaData, 1);
    while (sceSifDmaStat(temp) >= 0){}
    iosSignalIopSema();
    FlushCache(0);
}


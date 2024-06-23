#include <stdbool.h>

extern DAT_0013a300;
extern DAT_0013a380;

void entry(void)

{
  bool bVar1;
  char in_zero_lo;
  char in_zero_hi;
  char in_zero_udw;
  char in_register_0000000c;
  char *puVar2;
  char uVar3;

  puVar2 = &DAT_0013a300;
  do
  {
    *puVar2 = in_zero_lo;
    puVar2[1] = in_zero_hi;
    puVar2[2] = in_zero_udw;
    puVar2[3] = in_register_0000000c;
    bVar1 = puVar2 < (char *)0x16a518;
    puVar2 = puVar2 + 4;
  } while (bVar1);
  /* InitMainThread */
  syscall(0x3c);
  /* InitHeap */
  syscall(0x3d);
  FUN_0010fed8(0x16a518, 0x40000, 0x20000, 0x13a380, 0x1000c8);
  FlushCache(0);
  EI();
  uVar3 = FUN_00103b40(DAT_0013a380, 0x13a384);
  FUN_001102d8(uVar3);
  return;
}
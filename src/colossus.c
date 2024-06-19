void bosslifebar_sethp_sub_1401e48(float target_hp) {
  if (target_hp != DAT_01465a28) {
    if (target_hp < DAT_01465a28) {
      DAT_01465a38 = 1;
    }
    DAT_01465a34 = 0x3dcccccc;
    DAT_01465a2c = DAT_01465a30;
    DAT_01465a28 = target_hp;
    FUN_013fde00(3);
    return;
  }
  return;
}
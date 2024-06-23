float boss_current_hp;
int boss_hp_decreasing;

void bosslifebar_sethp_sub_1401e48(float new_hp)
{
  if (new_hp != boss_current_hp)
  {
    if (new_hp < boss_current_hp)
    {
      boss_hp_decreasing = 1;
    }
    DAT_01465a34 = 0x3dcccccc;
    DAT_01465a2c = DAT_01465a30;
    boss_current_hp = new_hp;
    FUN_013fde00(3);
    return;
  }
  return;
}
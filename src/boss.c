float boss_current_hp_ratio;
int boss_hp_decreasing;

void bosslifebar_sethp_sub_1401e48(float new_ratio)
{
  if (new_ratio != boss_current_hp_ratio)
  {
    if (new_ratio < boss_current_hp_ratio)
    {
      boss_hp_decreasing = 1;
    }
    FLOAT_01465a34 = 0.09999999;
    DAT_01465a2c = DAT_01465a30;
    boss_current_hp_ratio = new_ratio;
    FUN_013fde00(3);
    return;
  }
  return;
}
extern DAT_012e3d08; // Memory card pointer to time attack records for each colossus

float getBossIndexTimeAttackBestTime_sub_135e4b0(int boss_index)
{
  return (&DAT_012e3d08 + boss_index * 4);
}

void setBossIndexTimeAttackBestTime_sub_135e4c8(float new_best_time, int boss_index)
{
  ((int)&DAT_012e3d08 + boss_index * 4) = new_best_time;
  return;
}

extern DAT_01465bd0; // Time attack flag. 1 = time attack start, 0 = time attack stop

void SetTimeAttackTimeFlag_sub_1409060(int bool)
{
  DAT_01465bd0 = bool;
  return;
}

extern DAT_01464bd0;

void gStatStartTimeAttack_sub_13628e8(void)
{
  DAT_01464bd0 = 2;
  SetTimeAttackTimeFlag_sub_1409060(1);
  return;
}

void gStatStopTimeAttack_sub_1362910(void)
{
  DAT_01464bd0 = 3;
  SetTimeAttackTimeFlag_sub_1409060(0);
  return;
}

extern DAT_01465bd4; // Float value
extern DAT_01465bd8; // Float value

float GetTimeAttackTimeLimit_sub_1409070(void)
{
  return DAT_01465bd4 - DAT_01465bd8;
}

extern DAT_012912c4; // Likely boolean value
extern DAT_012912c8; // Likely boolean value

void mcSetCompleteTimeAttack_sub_1241348(long param_1, long param_2)

{
  if (param_1 != 0)
  {
    DAT_012912c4 = 1;
  }
  if (param_2 != 0)
  {
    DAT_012912c8 = 1;
  }
  return;
}
float player_hp;   // DAT_012e7b64
float player_grip; // DAT_012e7b60
float current_hp_ratio;
int hp_decreasing;

extern PTR_DAT_014649f8; // Memory card pointer multiplied by an index and number of bytes (4 for floats) to point to player max hp or player max grip

/*----------- Memory Card -----------------*/

float _gameFWorkChk_sub_135e388(int index)
{
  return (PTR_DAT_014649f8 + index * 4); // Return a float from the specified index in the memory card
}

void _gameFWorkSet_sub_135e328(float value, int index)
{
  (PTR_DAT_014649f8 + index * 4) = value; // Set specified value at specified index in memory card
  return;
}

/* ------------- Health -------------------*/

float getPlayerHp_sub_1367220(void)
{
  return player_hp;
}

float getPlayerMaxHp_sub_13673d0(void)
{
  float max_hp;

  max_hp = _gameFWorkChk_sub_135e388(0xb);
  return max_hp;
}

float _getPlayerHpRatio_sub_1367358(void)
{
  float current_hp;
  float max_hp;

  current_hp = getPlayerHp_sub_1367220();
  max_hp = getPlayerMaxHp_sub_13673d0();
  return current_hp / max_hp;
}

float getPlayerHpRatio_sub_1367390(void)
{
  float current_hp;
  float max_hp;

  current_hp = getPlayerHp_sub_1367220();
  max_hp = getPlayerMaxHp_sub_13673d0();
  return (current_hp / max_hp) * 100.0;
}

void playerlifebar_sethp_sub_1400088(float new_ratio)
{
  float fVar1;

  if (new_ratio != current_hp_ratio)
  {
    DAT_01465a0c = FLOAT_01465a10;
    fVar1 = new_ratio;
    if (new_ratio < current_hp_ratio)
    {
      FUN_013fde00(4);
      hp_decreasing = 1;
      fVar1 = FLOAT_01465a10;
    }
    FLOAT_01465a10 = fVar1;
    FLOAT_01465a14 = 0.09999999;
    current_hp_ratio = new_ratio;
    FUN_013fde00(1);
    return;
  }
  return;
}

void setPlayerHp_sub_1367230(float target_hp)
{
  float max_hp;
  float new_hp;

  max_hp = _gameFWorkChk_sub_135e388(0xb);
  if (max_hp < target_hp)
  {
    new_hp = _gameFWorkChk_sub_135e388(0xb);
  }
  else
  {
    new_hp = 0.0;
    if (0.0 <= target_hp)
    {
      new_hp = target_hp;
    }
  }
  player_hp = new_hp;
  new_hp = getPlayerHp_sub_1367220();
  max_hp = getPlayerMaxHp_sub_13673d0();
  playerlifebar_sethp_sub_1400088(new_hp / max_hp);
  return;
}

void addPlayerHp_sub_13672b0(float hp_to_add)
{
  float damage_ratio;
  float player_max_hp;
  float new_hp;

  new_hp = 0.0;
  if (hp_to_add < 0.0)
  {
    damage_ratio = gStatGetPlayerAccessoriesDamgeRaitoToPlayer_sub_1362938();
    hp_to_add = hp_to_add * damage_ratio;
  }
  hp_to_add = player_hp + hp_to_add;
  player_max_hp = _gameFWorkChk_sub_135e388(11);
  if (player_max_hp < hp_to_add)
  {
    player_hp = _gameFWorkChk_sub_135e388(11);
  }
  else
  {
    player_hp = new_hp;
    if (new_hp <= hp_to_add)
    {
      player_hp = hp_to_add;
    }
  }
  new_hp = getPlayerHp_sub_1367220();
  player_max_hp = getPlayerMaxHp_sub_13673d0();
  playerlifebar_sethp_sub_1400088(new_hp / player_max_hp);
  return;
}

void addPlayerMaxHp_sub_1367400(float hp_to_add)
{
  float max_hp = _gameFWorkChk_sub_135e388(0xb);
  _gameFWorkSet_sub_135e328(max_hp + hp_to_add, 0xb);
  playerlifebar_effect_sub_1401c78();
  return;
}

void endingflowDecPlayerHpPersent_sub_135cf08(float multiplier)
{
  float current_hp = getPlayerHp_sub_1367220();

  setPlayerHp_sub_1367230(current_hp * multiplier);
  return;
}

/* ------------- Grip -------------------*/

float getPlayerGrip_sub_1367438(void)
{
  return player_grip;
}

float getPlayerMaxGrip_sub_1367550()
{
  return _gameFWorkChk_sub_135e388(0xc);
}

void setPlayerGrip_sub_1367448(float target_grip)
{
  float current_grip = _gameFWorkChk_sub_135e388(0xc);
  if (current_grip < target_grip)
  {
    current_grip = _gameFWorkChk_sub_135e388(0xc);
  }
  else
  {
    current_grip = 0.0;
    if (0.0 <= target_grip)
    {
      current_grip = target_grip;
    }
  }
  player_grip = current_grip;
  float max_grip = _gameFWorkChk_sub_135e388(0xc);
  setgrip_sub_1401bd8(current_grip / max_grip);
  return;
}

void addPlayerGrip_sub_13674c8(float grip_to_add)
{
  float new_grip;
  float max_grip;

  grip_to_add = player_grip + grip_to_add;
  max_grip = _gameFWorkChk_sub_135e388(0xc);

  if (grip_to_add > max_grip)
  {
    new_grip = max_grip;
  }
  else
  {
    new_grip = 0.0;
    if (grip_to_add >= 0.0)
    {
      new_grip = grip_to_add;
    }
  }

  player_grip = new_grip;
  max_grip = _gameFWorkChk_sub_135e388(0xc);
  setgrip_sub_1401bd8(new_grip / max_grip);
  return;
}

extern DAT_014659e0; // Probably current grip ratio
extern DAT_014659e4; // Unknown
extern DAT_014659e8; // Unknown
extern DAT_014659d4; // Unknown
extern DAT_014659d8; // Unknown
extern DAT_014645c8; // Unknown

void setgrip_sub_1401bd8(float grip_ratio)
{
  if (grip_ratio == DAT_014659e0)
  {
    DAT_014659e4 = DAT_014659e8;
    DAT_014659d4 = 0;
    return;
  }
  if (grip_ratio < DAT_014659e0)
  {
    if (DAT_014659d4 == 0)
    {
      DAT_014659d8 = DAT_014645c8;
    }
    DAT_014659d4 = 1;
  }
  else
  {
    DAT_014659d4 = 0;
  }
  DAT_014659e4 = DAT_014659e8;
  DAT_014659e0 = grip_ratio;
  DAT_014659e8 = grip_ratio;
  FUN_013fde00(0);
  return;
}

void addPlayerMaxGrip_sub_1367580(float grip_to_add)
{
  float max_grip;

  max_grip = _gameFWorkChk_sub_135e388(0xc);
  _gameFWorkSet_sub_135e328(max_grip + grip_to_add, 0xc);
  startGaugeMaxAddEffect_sub_1401b48();
  return;
}

void startGaugeMaxAddEffect_sub_1401b48(void)
{
  FUN_013fde00(4);
  DAT_012feef4 = 0x3f199999;
  DAT_012fef00 = DAT_014645c8;
  DAT_012feef0 = 1;
  DAT_012feef8 = 0x3f000000;
  return;
}

void setPlayerMaxGrip_sub_1367568(float new_max)
{
  _gameFWorkSet_sub_135e328(new_max, 0xc);
  return;
}
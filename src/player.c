struct player_data {
  float max_hp;
  float max_grip;
};

struct player_data *player_data; // PTR_DAT_014649f8

float player_health; // DAT_012e7b64
float player_grip; // DAT_012e7b60

extern PTR_DAT_014649f8;


float _gameFWorkChk_sub_135e388(int param_1) {
  return *(float)(PTR_DAT_014649f8 + param_1 * 4);
}


/* ------------- Health -------------------*/


float getPlayerHp_sub_1367220(void) {
  return player_health;
}


float getPlayerMaxHp_sub_13673d0(void) {
  _gameFWorkChk_sub_135e388(0xb);
  return;
}


float _getPlayerHpRatio_sub_1367358(void) {
  float current_health = getPlayerHp_sub_1367220();
  float max_health = getPlayerMaxHp_sub_13673d0();

  return current_health / max_health;
}


void setPlayerHp_sub_1367230(float target_health) {
  float current_health;
  float max_health;
  
  current_health = (float)_gameFWorkChk_sub_135e388(0xb);
  if (current_health < target_health) {
    player_health = (float)_gameFWorkChk_sub_135e388(0xb);
  }
  else {
    player_health = 0.0;
    if (0.0 <= target_health) {
      player_health = target_health;
    }
  }
  current_health = getPlayerHp_sub_1367220();
  max_health = getPlayerMaxHp_sub_13673d0();
  playerlifebar_sethp_sub_1400088(current_health / max_health);
  return;
}


void addPlayerHp_sub_13672b0(float health_to_add) {
  float max_hp;
  float current_hp;
  
  current_hp = 0.0;
  if (health_to_add < 0.0) {
    max_hp = (float)gStatGetPlayerAccessoriesDamgeRaitoToPlayer_sub_1362938();
    health_to_add = health_to_add * max_hp;
  }
  health_to_add = player_health + health_to_add;
  max_hp = (float)_gameFWorkChk_sub_135e388(0xb);
  if (max_hp < health_to_add) {
    player_health = (float)_gameFWorkChk_sub_135e388(0xb);
  }
  else {
    player_health = current_hp;
    if (current_hp <= health_to_add) {
      player_health = health_to_add;
    }
  }
  current_hp = (float)getPlayerHp_sub_1367220();
  max_hp = (float)getPlayerMaxHp_sub_13673d0();
  playerlifebar_sethp_sub_1400088(current_hp / max_hp);
  return;
}


void endingflowDecPlayerHpPersent_sub_135cf08(float multiplier)

{
  float current_hp = getPlayerHp_sub_1367220();
  
  setPlayerHp_sub_1367230(current_hp * multiplier);
  return;
}



/* ------------- Grip -------------------*/

float getPlayerGrip_sub_1367438(void){
  return player_grip;
}


float getPlayerMaxGrip_sub_1367550() {
  return _gameFWorkChk_sub_135e388(0xc);
}


void setPlayerGrip_sub_1367448(float target_grip) {
  float current_grip;
  float max_grip;
  
  current_grip = _gameFWorkChk_sub_135e388(0xc);
  if (current_grip < target_grip) {
    current_grip = _gameFWorkChk_sub_135e388(0xc);
  }
  else {
    current_grip = 0.0;
    if (0.0 <= target_grip) {
      current_grip = target_grip;
    }
  }
  player_grip = current_grip;
  max_grip = _gameFWorkChk_sub_135e388(0xc);
  setgrip_sub_1401bd8(current_grip / max_grip);
  return;
}


void setgrip_sub_1401bd8(float param_1) {
  if (param_1 == DAT_014659e0) {
    DAT_014659e4 = DAT_014659e8;
    DAT_014659d4 = 0;
    return;
  }
  if (param_1 < DAT_014659e0) {
    if (DAT_014659d4 == 0) {
      DAT_014659d8 = DAT_014645c8;
    }
    DAT_014659d4 = 1;
  }
  else {
    DAT_014659d4 = 0;
  }
  DAT_014659e4 = DAT_014659e8;
  DAT_014659e0 = param_1;
  DAT_014659e8 = param_1;
  FUN_013fde00(0);
  return;
}

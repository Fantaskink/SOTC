float player_health; // DAT_012e7b64

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
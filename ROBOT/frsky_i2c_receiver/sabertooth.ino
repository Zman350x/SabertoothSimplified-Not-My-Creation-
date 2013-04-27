rc::Expo drive_expo;
rc::Expo turn_expo;

rc::DualRates turn_drates[3];


static int8_t ST_turn;
static int8_t ST_drive;

int8_t turn_drate_switch = 2;

#define MIN_INPUT 1090
#define MAX_INPUT 1910

void setupSabertooth() {

  turn_expo = 40;
  drive_expo = 40;
  
  turn_drates[0] = 50;
  turn_drates[1] = 75;
  turn_drates[2] = 100;
  
  ST_turn = 0;
  ST_drive = 0;

  ST.autobaud();
  ST.setDeadband(3);
  
}


void processSabertooth() {

  if (!MOTORSTOP) {
    switchExpo();
    
    int16_t drive_in = constrain(i2c_dataset.c2, MIN_INPUT, MAX_INPUT);
    int16_t turn_in = constrain(i2c_dataset.c1, MIN_INPUT, MAX_INPUT);
    
    int16_t normalized_drive = map(drive_in, MIN_INPUT,MAX_INPUT, -256, 256);
    int16_t normalized_turn = map(turn_in, MIN_INPUT,MAX_INPUT, -256, 256);
    normalized_drive = drive_expo.apply(normalized_drive);
    //normalized_turn = turn_expo.apply(normalized_turn);
    
    int16_t rated_turn = turn_drates[turn_drate_switch].apply(normalized_turn);
    i2c_dataset.drive = map(normalized_drive, -256, 256, -127, 127);
    i2c_dataset.turn = map(normalized_turn, -256, 256, -127, 127);
    ST_turn = i2c_dataset.turn;
    ST_drive = i2c_dataset.drive; 
  } else {
    ST_turn = 0;
    ST_drive = 0;
  }
  
  if (!DEBUGMODE) {
    ST.drive(ST_drive);
    ST.turn(ST_turn);
  }
  
  
  /*
  Serial.print(ST_turn);
  Serial.print("-");
  Serial.print(ST_drive);
  Serial.println();
  */
}

void switchExpo() {
  if (i2c_dataset.c6 < 1100) { // OBEN
    turn_drate_switch = 0;
    //Serial.print("P1-");
  } else if(i2c_dataset.c6 > 1200 && i2c_dataset.c6 < 1600) { // MITTE
    turn_drate_switch = 1;
    //Serial.print("P2-");
  } else { // UNTEN
    turn_drate_switch = 2;
    //Serial.print("P3-");
  }
}

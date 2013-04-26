rc::Expo drive_expo;
rc::Expo turn_expo;

static int8_t ST_turn;
static int8_t ST_drive;

#define MIN_INPUT 1090
#define MAX_INPUT 1910

void setupSabertooth() {

  turn_expo = 40;
  drive_expo = 40;
  ST_turn = 0;
  ST_drive = 0;

  ST.autobaud();
  ST.setDeadband(3);
  
}


void processSabertooth() {

  if (!MOTORSTOP) {
    switchExpo();
    //Serial.println(i2c_dataset.c1);
    
    
    int16_t drive_in = constrain(i2c_dataset.c2, MIN_INPUT, MAX_INPUT);
    int16_t turn_in = constrain(i2c_dataset.c1, MIN_INPUT, MAX_INPUT);
    
    int16_t normalized_drive = map(drive_in, MIN_INPUT,MAX_INPUT, -256, 256);
    int16_t normalized_turn = map(turn_in, MIN_INPUT,MAX_INPUT, -256, 256);
    normalized_drive = drive_expo.apply(normalized_drive);
    normalized_turn = turn_expo.apply(normalized_turn);
    i2c_dataset.drive = map(normalized_drive, -256, 256, -127, 127);
    i2c_dataset.turn = map(normalized_turn, -256, 256, -127, 127);
    ST_turn = i2c_dataset.turn;
    ST_drive = i2c_dataset.drive; 
  } else {
    Serial.println("MOTORSTOP");
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
  if (i2c_dataset.c6 < 1000) {
    //Serial.print("P1-");
  } else if(i2c_dataset.c6 > 1000 && i2c_dataset.c6 < 1600) {
    //Serial.print("P2-");
  } else {
    //Serial.print("P3-");
  }
}

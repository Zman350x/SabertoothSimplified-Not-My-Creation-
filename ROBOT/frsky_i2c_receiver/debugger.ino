static uint8_t currentdebugmenu = 0;

void processInput() {
  if (DEBUGMODE) {
    if (DEBUGMODE && inputString == "exit\n") {
      Serial.println(F("Debugmode off!"));
      DEBUGMODE = false;
      DEBUGMENUPAGE = 0;
    } else if (DEBUGMENUPAGE != 0 && inputString == "-\n") {
      debugMenuMain();
    } else if (DEBUGMENUPAGE == 0) { // We are in Debug Mainmenu
      if (inputString == "1\n"){
        debugRcInput();
      }
    } else if (DEBUGMENUPAGE == 1) {
      String input = String(inputString);
      if (input.startsWith("turn")) {
        uint8_t chan = (int)input.charAt(5)-48;
        uint8_t reverse = (int)input.charAt(7)-48;

        if (reverse != 0 && reverse != 1) {
          reverse = 0;
        }
        
        if (chan >= 1 && chan <= 8) {
          Serial.print(F(" >>> "));
          Serial.print(F("Setting turn to Channel "));
          Serial.print(chan);
          if (reverse == 1) Serial.print(F(" - reversed"));
          Serial.println(F(" >>> "));
          i2c_dataset.rcmapping.turn_chan = (chan-1);
          i2c_dataset.rcmapping.turn_reverse = reverse;
          debugRcInput();
        }
        
      } else if(input.startsWith("drive")) {
        Serial.println(input);
      }
    }
  } else if (inputString == "+++\n") {
    debugMenuMain();
    DEBUGMODE = true;
  }
}

// DEBUGMENUPAGE = 1
void debugMenuMain() {
  DEBUGMENUPAGE = 0;
  Serial.println(F("--------------------------------------------------------------"));
  Serial.println(F("DEBUG ON"));
  Serial.println(F("--------------------------------------------------------------"));
  Serial.println();
  Serial.println(F(" 1 = RC Input parameters"));
  Serial.println();
  Serial.println(F(" exit = Debugmode off"));
  Serial.println(F("--------------------------------------------------------------"));
}

void debugRcInput() {
  DEBUGMENUPAGE = 1;
  Serial.println(F("--------------------------------------------------------------"));
  Serial.println(F(" RC INPUT "));
  Serial.println(F("--------------------------------------------------------------"));
  Serial.println(F("               CHANNEL"));
  Serial.println(F("           1 2 3 4 5 6 7 8  REV"));
  Serial.print(  F(" drive :   "));
  for(uint8_t x = 0; x<8; x++) {
    if (i2c_dataset.rcmapping.drive_chan == x)
      Serial.print(F("X "));
    else
      Serial.print(F("  "));
  }
  if (i2c_dataset.rcmapping.drive_reverse == 0x00)
    Serial.println(F("  0"));
  else
    Serial.println(F("  1"));

  Serial.print(  F(" turn  :   "));
  for(uint8_t x = 0; x<8; x++) {
    if (i2c_dataset.rcmapping.turn_chan == x)
      Serial.print(F("X "));
    else
      Serial.print(F("  "));
  }
  if (i2c_dataset.rcmapping.turn_reverse == 0x00)
    Serial.println(F("  0"));
  else
    Serial.println(F("  1"));

  Serial.println();
  Serial.println(F(" Change turn to channel 3 and reverse: turn 3 1 <enter>"));
  Serial.println(F(" - = back / exit = Debugmode off"));
  Serial.println();
  Serial.println(F("--------------------------------------------------------------"));  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}

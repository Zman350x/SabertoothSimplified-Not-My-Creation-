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
  Serial.println();
  Serial.println();
  Serial.println();
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

static uint8_t currentdebugmenu = 0;

void processInput() {
  if (DEBUGMODE) {
    if(inputString == "-\n") {
        Serial.println("BACK");
    }
  } else if (inputString == "+++\n") {
    Serial.println("--- frsky_i2c_receiver");
    Serial.println("DEBUG ON");
    DEBUGMODE = true;
  }
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


void processRC() {
  if (g_PPMIn.isStable()) {
    i2cResponse.stateregister = 0x01;
    i2cResponse.c1 = g_values[0];
    i2cResponse.c2 = g_values[1];
    i2cResponse.c3 = g_values[2];
    i2cResponse.c4 = g_values[3];
    
    Serial.print(g_values[0]);
    Serial.print(" - ");
    Serial.print(g_values[1]);
    Serial.print(" - ");
    Serial.print(g_values[2]);
    Serial.print(" - ");
    Serial.print(g_values[3]);
    Serial.println();
    
    // do magic, incoming values available in g_values in microseconds.
  } else if (g_PPMIn.isLost()) {
    bitClear(registers.stateregister, 3);
    // signal has been lost (no new valid frames for 'timeout' milliseconds)
  } else {
    bitClear(registers.stateregister, 3);
  }
}

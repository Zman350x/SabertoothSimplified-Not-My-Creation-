
void processRC() {
  if (g_PPMIn.isStable()) {
    registers.stateregister = 0x01;
    registers.c1 = g_values[0];
    registers.c2 = g_values[1];
    registers.c3 = g_values[2];
    registers.c4 = g_values[3];
    registers.c5 = g_values[4];
    registers.c6 = g_values[5];
    registers.c7 = g_values[6];
    registers.c8 = g_values[7];
    /*
    Serial.print(g_values[0]);
    Serial.print(" - ");
    Serial.print(g_values[1]);
    Serial.print(" - ");
    Serial.print(g_values[2]);
    Serial.print(" - ");
    Serial.print(g_values[3]);
    Serial.print(" - ");
    Serial.print(g_values[4]);
    Serial.print(" - ");
    Serial.print(g_values[5]);
    Serial.print(" - ");
    Serial.print(g_values[6]);
    Serial.print(" - ");
    Serial.print(g_values[7]);
    Serial.println();
    */
    // do magic, incoming values available in g_values in microseconds.
  } else if (g_PPMIn.isLost()) {
    bitClear(registers.stateregister, 3);
    // signal has been lost (no new valid frames for 'timeout' milliseconds)
  } else {
    bitClear(registers.stateregister, 3);
  }
}

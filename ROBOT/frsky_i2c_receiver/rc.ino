
void processRC() {
  if (g_PPMIn.isStable()) {
    MOTORSTOP = false;
    i2c_dataset.stateregister.signal = 1;
    i2c_dataset.c1 = g_values[0];
    i2c_dataset.c2 = g_values[1];
    i2c_dataset.c3 = g_values[2];
    i2c_dataset.c4 = g_values[3];
    i2c_dataset.c5 = g_values[4];
    i2c_dataset.c6 = g_values[5];
    //i2c_dataset.c7 = g_values[6];
    //i2c_dataset.c8 = g_values[7];
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
    i2c_dataset.stateregister.signal = 0;
    MOTORSTOP = true;
    // signal has been lost (no new valid frames for 'timeout' milliseconds)
  } else {
    i2c_dataset.stateregister.signal = 0;
    MOTORSTOP = true;
  }
}

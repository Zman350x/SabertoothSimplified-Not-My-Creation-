
void i2cRequestEvent() {
  uint8_t startIndex = i2cRecBuffer[0];
  uint8_t byteCount = sizeof(registers);

  if (startIndex < byteCount) {
    
    // Copy to byte Buffer
    byte buf[byteCount];
    memcpy(&buf, &registers, byteCount);

    byte shift = i2cRecBuffer[0];  // Startindex
    byte temp[byteCount-shift];
    
    for (int i=shift; i<byteCount; i++) {
      temp[i-shift] = buf[i];
    }
    Wire.write(temp, sizeof temp);
  } else {
    Wire.write(0x00);
  }

  //Wire.write (buf, sizeof buf);
  //Wire.write ((byte *) &i2cResponse, sizeof i2cResponse);
}

void i2cReceiveEvent(int numBytes) {
  for (int a = 0; a < numBytes; a++) {
    if (a < I2CMAX_IN_BYTES) {
      i2cRecBuffer[a] = Wire.read();
    } else {
      Wire.read();
    }
  }
  i2cReceive = true;
  return;
}

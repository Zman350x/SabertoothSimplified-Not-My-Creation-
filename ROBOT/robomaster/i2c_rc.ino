


void i2c_rc_read() {
  Wire.requestFrom(RC_I2C_ADDR, 21);    // request 21 bytes from slave device
  uint8_t *ptr = (uint8_t *)&rc_i2c_dataset;
  while(Wire.available())    // slave may send less than requested
  { 
    uint8_t c = Wire.read(); // receive a byte as character
    *ptr++ = c;
  }
  Serial.print(rc_i2c_dataset.c1);
  Serial.print(" - ");
  Serial.print(rc_i2c_dataset.drive);
  Serial.print(" - ");
  Serial.print(rc_i2c_dataset.turn);
  Serial.println();
  
}

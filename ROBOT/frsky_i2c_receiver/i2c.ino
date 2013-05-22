
//////////////////////////////////////////////////////////////////////////////////////
// I2C handlers
// Handler for requesting data
//
void i2cRequestEvent()
{
 //if (receivedCommands[0] >= I2C_GPS_GROUND_SPEED) i2c_dataset.status.new_data = 0;        //Accessing gps data, switch new_data_flag;
 //Write data from the requested data register position
 Wire.write((uint8_t *)&i2c_dataset+receivedCommands[0],32);                    //Write up to 32 byte, since master is responsible for reading and sending NACK
 //32 byte limit is in the Wire library, we have to live with it unless writing our own wire library
}

//Handler for receiving data
void i2cReceiveEvent(int bytesReceived) {
  uint8_t  *ptr;
  for (int a = 0; a < bytesReceived; a++) {
    if (a < I2CMAX_IN_BYTES) {
      receivedCommands[a] = Wire.read();
    } else {
      Wire.read();  // if we receive more data then allowed just throw it away
    }
  }
/*
    if (receivedCommands[0] == I2C_GPS_COMMAND)
    {
        new_command = receivedCommands[1];    //Just one byte, ignore all others
        return;
    }
*/
  if (bytesReceived == 1 && (receivedCommands[0] < REG_MAP_SIZE)) {
    return;    //read command from a given register
  }
 
  if (bytesReceived == 1 && (receivedCommands[0] >= REG_MAP_SIZE)) {                 //Addressing over the reg_map fallback to first byte
    receivedCommands[0] = 0x00;
    return;
  }
    //More than 1 byte was received, so there is definitely some data to write into a register
    //Check for writeable registers and discard data is it's not writeable

  if ((receivedCommands[0] >= 0x11) && (receivedCommands[0] <= 0x15)) { //Writeable registers above 0x10  
    ptr = (uint8_t *)&i2c_dataset + receivedCommands[0];
    for (int a = 1; a < bytesReceived; a++) {
      *ptr++ = receivedCommands[a];
    }
  }
}


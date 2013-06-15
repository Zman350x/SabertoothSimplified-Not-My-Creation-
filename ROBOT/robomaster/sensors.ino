void initSensors() {
  //dht.begin();
  
  initDHT();
}

void initDHT() {
  DDRE |= _BV(DHT11_PIN);
  PORTE |= _BV(DHT11_PIN);
}

long dht11delay_previousMillis = 0;        // will store last time LED was updated
long dht11delay_interval = DHT_READ_INTERVAL;           // dht11delay_interval at which to blink (milliseconds)

int temp1[3];                //Temp1, temp2, hum1 & hum2 are the final integer values that you are going to use in your program. 
int temp2[3];                // They update every 2 seconds.
int hum1[3];
int hum2[3];

float bat_voltage_store = 0;

float readBatVoltage() {
  long InternalVcc = readVccArduinoMega();

  double sensorValue = analogRead(VOLT_SENS_PORT);
  double SensedVoltageV  = ((sensorValue * InternalVcc) / 1024 /1000)-0.21;
  double SensedVoltageMv  = ((sensorValue * InternalVcc) / 1024)-210;
  //Serial.println(((sensorValue * InternalVcc) / 1024));
  float calculatedVoltage = SensedVoltageMv * VOLT_SENS_MULTIPLIKATOR / 1000;
  bat_voltage_store = ((bat_voltage_store*9) + calculatedVoltage)/10;
  //Serial.println(calculatedVoltage);
  BAT_VoltCur.voltage = bat_voltage_store*100;
  /*
  Serial.print("VCC: ");
  Serial.print(InternalVcc);
  Serial.print(" - sensor = " );                       
  Serial.print(sensorValue);
  Serial.print(" - mv:");
  Serial.print(SensedVoltageMv);
  Serial.print(" - V:");
  Serial.print(SensedVoltageV);
  Serial.print(" - ");
  Serial.print(bat_voltage_store);
  Serial.println();
  */
  return bat_voltage_store;
}

double cur_left_store = 0.0;
double cur_right_store = 0.0;

void readCurrent() {
  double cur_left = currentSensor(analogRead(CUR_SENS_LEFT_PIN));
  double cur_right = currentSensor(analogRead(CUR_SENS_RIGHT_PIN));
  if (cur_left < 0)
    cur_left = cur_left *-1.0;
  if (cur_right < 0)
    cur_right = cur_right *-1.0;
  cur_left_store = ((cur_left_store*9)+cur_left)/10;
  cur_right_store = ((cur_right_store*9)+cur_right)/10;
#ifdef DEBUG_SENSOR_CURRENT
  Serial.print("L:");
  Serial.print(cur_left_store);
  Serial.print(" R:");
  Serial.println(cur_right_store);
#endif
  BAT_VoltCur.cur_left  = cur_left_store * 100;
  BAT_VoltCur.cur_right = cur_right_store * 100;
}

static double currentSensor(int RawADC) {

  int    Sensitivity    = 66; // mV/A

  long   InternalVcc    = readVccArduinoMega();
  double ZeroCurrentVcc = InternalVcc / 2;
  double SensedVoltage  = (RawADC * InternalVcc) / 1024;
  double Difference     = SensedVoltage - ZeroCurrentVcc;
  double SensedCurrent  = Difference / Sensitivity;
  /*
  Serial.print("ADC: ");
  Serial.print(RawADC);
  Serial.print("/1024");

  Serial.print(", Sensed Voltage: ");
  printDouble(SensedVoltage, 1);
  Serial.print("mV");

  Serial.print(", 0A at: ");
  printDouble(ZeroCurrentVcc, 1);
  Serial.print("mV, ");
  */
  return SensedCurrent;                                        // Return the Current

} 


void readDHT() {
  unsigned long dht11delay_currentMillis = millis();

  if(dht11delay_currentMillis - dht11delay_previousMillis > dht11delay_interval) {
    // save the last time you blinked the LED 
    dht11delay_previousMillis = dht11delay_currentMillis;   
    byte dht11_dat[5];
    byte dht11_in;
    byte i;
    // start condition
    // 1. pull-down i/o pin from 18ms
    PORTE &= ~_BV(DHT11_PIN);
    delay(18);
    PORTE |= _BV(DHT11_PIN);
    delayMicroseconds(40);

    DDRE &= ~_BV(DHT11_PIN);
    delayMicroseconds(40);

    dht11_in = PINE & _BV(DHT11_PIN);

    if(dht11_in){
      Serial.println("dht11 start condition 1 not met");
      return;
    }
    delayMicroseconds(80);

    dht11_in = PINE & _BV(DHT11_PIN);

    if(!dht11_in){
      Serial.println("dht11 start condition 2 not met");
      return;
    }
    delayMicroseconds(80);
    // now ready for data reception
    for (i=0; i<5; i++)
      dht11_dat[i] = read_dht11_dat();

    DDRE |= _BV(DHT11_PIN);
    PORTE |= _BV(DHT11_PIN);

    byte dht11_check_sum = dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3];
    // check check_sum
    if(dht11_dat[4]!= dht11_check_sum)
    {
      Serial.println("DHT11 checksum error");
    }

    DHT_Vals.temp = dht11_dat[2];
    DHT_Vals.hum = dht11_dat[0];
    
    temp1[0]=dht11_dat[2];
    temp2[0]=dht11_dat[3];
    hum1[0]=dht11_dat[0];
    hum2[0]=dht11_dat[1];
#ifdef DEBUG_SENSOR_DHT
    Serial.print("Temperature: ");
    Serial.print(temp1[0]);
    Serial.print(".");
    Serial.print(temp2[0]);
    Serial.print(" C");
    Serial.print("    ");
    Serial.print("Humidity: ");
    Serial.print(hum1[0]);
    Serial.print(".");
    Serial.print(hum2[0]);
    Serial.println("%");
#endif
    
  }
}

byte read_dht11_dat() {
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++){
    while(!(PINE & _BV(DHT11_PIN)));  // wait for 50us
    delayMicroseconds(30);
    if(PINE & _BV(DHT11_PIN)) 
      result |=(1<<(7-i));
    while((PINE & _BV(DHT11_PIN)));  // wait '1' finish
  }
  return result;
}


// reads vcc in mV
static long readVccArduinoMega() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRB &= ~_BV(MUX5);
  delay(5); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

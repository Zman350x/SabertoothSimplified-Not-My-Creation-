
#include <Wire.h>
#include <PPMIn.h>
#include <Timer1.h>
#include <Curve.h>
#include <Expo.h>

#include <Sabertooth.h>

Sabertooth ST(128);

#define RCCHANNELS 7
#define I2CADR 0x18
#define I2CMAX_IN_BYTES 5

static uint8_t         receivedCommands[I2CMAX_IN_BYTES];
static uint8_t         new_command;                        //new command received (!=0)

static boolean MOTORSTOP = true;
static boolean DEBUGMODE = false;

uint16_t g_values[RCCHANNELS];                  // output buffer for PPMIn
uint8_t  g_workIn[PPMIN_WORK_SIZE(RCCHANNELS)]; // we need to have a work buffer for the PPMIn class

rc::PPMIn g_PPMIn(g_values, g_workIn, RCCHANNELS);

#define REG_MAP_SIZE       sizeof(i2c_dataset)       //size of register map

typedef struct {
  uint8_t state: 1;
  uint8_t debug: 1;
  uint8_t override: 1;
  uint8_t signal: 1;
} STATUS_REGISTER;

typedef struct {
  uint8_t drive_chan: 3;
  uint8_t drive_reverse: 1;
  uint8_t turn_chan: 3;
  uint8_t turn_reverse: 1;
} RCMAPPING_REGISTER;

typedef struct {
  STATUS_REGISTER stateregister; // 0x00
  uint16_t c1;        // 0x01 & 0x02
  uint16_t c2;        // 0x03 & 0x04
  uint16_t c3;        // 0x05 & 0x06
  uint16_t c4;        // 0x07 & 0x08
  uint16_t c5;        // 0x09 & 0x0A
  uint16_t c6;        // 0x0B & 0x0C
  uint16_t c7;        // 0x0D & 0x0E
  uint16_t c8;        // 0x0F & 0x10
  int8_t drive;       // 0x11
  int8_t turn;        // 0x12
  RCMAPPING_REGISTER rcmapping; // 0x13
} I2C_REGISTERS;

static I2C_REGISTERS i2c_dataset;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  // Initialize timer1, this is required for all features that use Timer1
  // (PPMIn/PPMOut/ServoIn/ServoOut)
  rc::Timer1::init();
  // We use pin 8 as PPM input pin
  pinMode(8, INPUT);

  // We use pin change interrupts to detect changes in the signal
  // If you're unfamiliar with how this works, please look up some
  // article or tutorial on the subject.

  // only allow pin change interrupts for PB0 (digital pin 8)
  PCMSK0 = (1 << PCINT0);

  // enable pin change interrupt 0
  PCICR = (1 << PCIE0);


  
  Serial.begin(9600);  // start serial for output
  
  setupSabertooth();
  // set a timeout (default 500 milliseconds)
  g_PPMIn.setTimeout(500);
  g_PPMIn.setPauseLength(3000); // 4000 norm
  // start listening
  g_PPMIn.start();

//Init i2c_dataset;
  uint8_t *ptr = (uint8_t *)&i2c_dataset;
  for (uint8_t i = 0; i < sizeof(i2c_dataset); i++) {
      *ptr = 0;
      ptr++;
  }

  i2c_dataset.rcmapping.turn_chan = 0x01; // Turn Channel=2
  i2c_dataset.stateregister.state = 0;

  // START I2C
  Wire.begin(I2CADR);        // join i2c bus (address optional for master)
  //TWBR = 12;
  Wire.onRequest(i2cRequestEvent);
  Wire.onReceive(i2cReceiveEvent);  
  
  inputString.reserve(200);
}

void loop(){
  // update incoming values
  g_PPMIn.update();
  processRC();
    
  if (stringComplete) {
    processInput();
    inputString = "";
    stringComplete = false;
  }
  
  if(i2c_dataset.c5 < 1500) {
    MOTORSTOP = true;
  }
  
  processSabertooth();
  if (new_command) {
    new_command = false;
    //Serial.print("i2cReceive:");
    //Serial.println(i2cRecBuffer[0]);
  }
}

// Interrupt handling code below, this needs cleaning

static uint8_t lastB = 0; // last read value of PINB

// Pin change port 0 interrupt
ISR(PCINT0_vect){
  uint8_t newB = PINB;

  // we've hardcoded the bitmask here (1 << 0), the portable way to get it is
  // digitalPinToBitMask(pinnr), but you really don't want to call that in
  // the interrupt handler.
  // we test if the specified pin has changed (not really necessary, but
  // think about what would happen if you were monitoring multiple pins)
  if ((lastB & (1 << 0)) != (newB & (1 << 0))){
    // and we tell PPMIn the pin has changed
    g_PPMIn.pinChanged(newB & (1 << 0));
  }

  lastB = newB;
}

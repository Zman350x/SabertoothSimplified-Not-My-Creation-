
#include <Wire.h>
#include <PPMIn.h>
#include <Timer1.h>

#include <Sabertooth.h>

//Sabertooth ST(128);

#define RCCHANNELS 4
#define I2CADR 0x18
#define I2CMAX_IN_BYTES 5

byte i2cRecBuffer[I2CMAX_IN_BYTES];
boolean i2cReceive = false;
uint16_t g_values[RCCHANNELS];                  // output buffer for PPMIn
uint8_t  g_workIn[PPMIN_WORK_SIZE(RCCHANNELS)]; // we need to have a work buffer for the PPMIn class

rc::PPMIn g_PPMIn(g_values, g_workIn, RCCHANNELS);

struct {
  byte stateregister; // 0x00
  uint16_t c1;        // 0x01 & 0x02
  uint16_t c2;        // 0x03 & 0x04
  uint16_t c3;        // 0x05 & 0x06
  uint16_t c4;        // 0x07 & 0x08
  uint16_t c5;        // 0x09 & 0x0A
  uint16_t c6;        // 0x0B & 0x0C
  int8_t drive;       // 0x0D
  int8_t turn;        // 0x0E
} registers;

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

  // set a timeout (default 500 milliseconds)
  g_PPMIn.setTimeout(500);

  Wire.begin(I2CADR);        // join i2c bus (address optional for master)
  //TWBR = 12;
  Wire.onRequest(i2cRequestEvent);
  Wire.onReceive(i2cReceiveEvent);
  Serial.begin(9600);  // start serial for output
  Serial.println("frsky_i2c_receiver");
  //ST.autobaud();
  //Serial.println("frsky_i2c_receiver");

  // start listening
  g_PPMIn.start();

}

void loop(){
  // update incoming values
  g_PPMIn.update();
  processRC();
  bitSet(registers.stateregister, 0);
  
  if (i2cReceive) {
    i2cReceive = false;
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


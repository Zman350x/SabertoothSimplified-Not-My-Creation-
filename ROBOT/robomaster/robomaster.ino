#include "config.h"
#include <FastSerial.h>
#include "../mavlink/include/mavlink.h"        // Mavlink interface
FastSerialPort0(Serial);
FastSerialPort1(Serial1);
FastSerialPort2(Serial2);
FastSerialPort3(Serial3);

typedef struct {
  int16_t temp;
  int16_t hum;
} DHT_VALUES;

typedef struct {
  uint16_t voltage;
  uint16_t cur_left;
  uint16_t cur_right;
} VOLTAGECURRENT;

static DHT_VALUES DHT_Vals;
static VOLTAGECURRENT BAT_VoltCur;

void setup() {
  Serial.begin(DEBUG_BAUD);
  Serial3.begin(TELPORT_BAUD);
  Serial.println("ROBOMASTER");
  initSensors();
  
}

////////////////////////////////////////////////////////////////////////////////
// System Timers
////////////////////////////////////////////////////////////////////////////////
// Time in miliseconds of start of main control loop.  Milliseconds
static unsigned long    fast_loopTimer;
// Time Stamp when fast loop was complete.  Milliseconds
static unsigned long    fast_loopTimeStamp;
// Number of milliseconds used in last main loop cycle
static uint8_t          delta_ms_fast_loop;
// Counter of main loop executions.  Used for performance monitoring and failsafe processing
static uint16_t         mainLoop_count;
// Time in miliseconds of start of medium control loop.  Milliseconds
static unsigned long    medium_loopTimer;
// Counters for branching from main control loop to slower loops
static byte             medium_loopCounter; 
// Number of milliseconds used in last medium loop cycle
static uint8_t          delta_ms_medium_loop;
// Counters for branching from medium control loop to slower loops
static byte             slow_loopCounter;
// Counter to trigger execution of very low rate processes
static byte             superslow_loopCounter;
// Counter to trigger execution of 1 Hz processes
static byte             counter_one_herz;
// Counter to trigger execution of 5 Hz processes
static byte             counter_five_herz;
// % MCU cycles used
static float            load;

////////////////////////////////////////////////////////////////////////////////
// Performance monitoring
////////////////////////////////////////////////////////////////////////////////
// Timer used to accrue data and trigger recording of the performanc monitoring log message
static long     perf_mon_timer;

void loop() {
  if (millis()-fast_loopTimer > 19) {
    delta_ms_fast_loop  = millis() - fast_loopTimer;
    load                = (float)(fast_loopTimeStamp - fast_loopTimer)/delta_ms_fast_loop;
    //G_Dt                = (float)delta_ms_fast_loop / 1000.f;
    fast_loopTimer      = millis();
    mainLoop_count++;

    // Execute the fast loop
    // ---------------------
    fast_loop();

    // Execute the medium loop
    // -----------------------
    medium_loop();

    counter_one_herz++;
    if(counter_one_herz >= 50){
      one_second_loop();
      counter_one_herz = 0;
    }

    counter_five_herz++;
    if(counter_five_herz >= 250){
      five_second_loop();
      counter_five_herz = 0;
    }

    if (millis() - perf_mon_timer > 20000) {
      if (mainLoop_count != 0) {
        resetPerfData();
      }
    }

    fast_loopTimeStamp = millis();
  }
}

void fast_loop() {
  mavlink_receive();
}

void medium_loop() {
  // This is the start of the medium (10 Hz) loop pieces
  // -----------------------------------------
  switch(medium_loopCounter) {
    // This case deals with the GPS
    //-------------------------------
    case 0:
      medium_loopCounter++;
    break;
    
    // This case performs some navigation computations
    //------------------------------------------------
    case 1:
      medium_loopCounter++;
    break;

    // command processing
    //------------------------------
    case 2:
      medium_loopCounter++;
    break;

    // This case deals with sending high rate telemetry
    //-------------------------------------------------
    case 3:
      medium_loopCounter++;
      // Send Voltage & Current
      send_mav_voltage_current();

    break;

    // This case controls the slow loop
    //---------------------------------
    case 4:
      medium_loopCounter      = 0;
      delta_ms_medium_loop    = millis() - medium_loopTimer;
      medium_loopTimer        = millis();
      slow_loop();
    break;
  }
  readDHT();
  readBatVoltage();
  readCurrent();

}


static void slow_loop() {
  // This is the slow (3 1/3 Hz) loop pieces
  //----------------------------------------
  switch (slow_loopCounter){
    case 0:
      slow_loopCounter++;
      superslow_loopCounter++;
      if(superslow_loopCounter >=200) {  //      200 = Execute every minute
        superslow_loopCounter = 0;
      }
    break;

    case 1:
      slow_loopCounter++;
    break;

    case 2:
      slow_loopCounter = 0;
    break;
  }
}

void one_second_loop() {
  mavlink_run();
}

void five_second_loop() {
  // Send DHT Values
  send_mav_dht_values();
}

static void resetPerfData(void) {
  mainLoop_count             = 0;
  //G_Dt_max                   = 0;
  //imu.adc_constraints      = 0;
  //ahrs.renorm_range_count  = 0;
  //ahrs.renorm_blowup_count = 0;
  //gps_fix_count            = 0;
  //pmTest1                  = 0;
  perf_mon_timer             = millis();
}

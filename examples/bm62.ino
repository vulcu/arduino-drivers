#include <Wire.h>

#include "BM62.h"

#define SERIAL_TIMEOUT_MILLISECONDS (1000U)
#define SERIAL_BAUD_RATE (115200U)

// define the digital I/O for various drivers
#define RST_N           ( 8U)
#define IND_A2DP_N      (14U)
#define PRGM_SENSE_N    (17U)

// create BM62 driver object
BM62::BM62 bluetooth(PRGM_SENSE_N, RST_N, IND_A2DP_N, &Serial);

void setup() {
  // initialize serial console and set baud rate
  Serial.setTimeout(SERIAL_TIMEOUT_MILLISECONDS);
  Serial.begin(SERIAL_BAUD_RATE);

  // example driver initialization goes here
  bluetooth.init();
}

void loop() {
    // main code goes here
}
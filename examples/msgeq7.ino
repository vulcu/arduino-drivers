#include <Wire.h>

#include "MSGEQ7.h"

// define the digital I/O for various drivers
#define STROBE          ( 9U)
#define RESET           (10U)
#define DC_OUT          ( A1)

#define MSGEQ7_INPUT_PULLUP_ON_DC_OUT (false)
#define MSGEQ7_SIGNAL_BAND_COUNT      ((uint8_t)7U)
#define MSGEQ7_ZERO_SIGNAL_LEVEL      ((uint16_t)400U)

// create MSGEQ7 driver object
MSGEQ7::MSGEQ7 spectrum(STROBE, DC_OUT, RESET, MSGEQ7_INPUT_PULLUP_ON_DC_OUT);

void setup() {
  // example driver initialization goes here
  spectrum.init();
}

void loop() {
    // main code goes here
}
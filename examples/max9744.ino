#include <Wire.h>

#include "MAX9744.h"

#define I2C_CLOCK_RATE   (400000U)

// define the digital I/O for various drivers
#define MUTE            (12U)
#define SHDN            (13U)

// create MAX9744 driver object
MAX9744::MAX9744 amplifier(MAX9744_DEFAULT_I2CADDR, MUTE, SHDN, &Wire);

void setup() {

  // initialize Wire library and set clock rate to 400 kHz
  Wire.begin();
  Wire.setClock(I2C_CLOCK_RATE);

  // example driver initialization goes here
  amplifier.init();
}

void loop() {
    // main code goes here
}
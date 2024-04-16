# arduino-drivers
Hardware and IC driver collection for use with Arduino

# Usage
```cpp
#include <Wire.h>

#include "audio/AK5558.h"
#include "audio/CS4270.h"
#include "audio/DS1882.h"
#include "audio/MAX9744.h"
#include "audio/MSGEQ7.h"
#include "bluetooth/BM62.h"
#include "io/buttonled.h"
#include "io/PCAL6408A.h"

#define SERIAL_TIMEOUT_MILLISECONDS (1000U)
#define SERIAL_BAUD_RATE (115200U)
#define I2C_CLOCK_RATE   (400000U)

// define the digital I/O for various drivers
#define PCAL6408A_INT_L ( 2U)
#define BUTTON_PIN      ( 3U)
#define AK5558_RESET_L  ( 4U)
#define LED_PIN         ( 5U)
#define PCAL6408A_RST_L ( 6U)
#define DS1882_EN_N     ( 7U)
#define RST_N           ( 8U)
#define STROBE          ( 9U)
#define RESET           (10U)
#define CS4270_RST_N    (11U)
#define MUTE            (12U)
#define SHDN            (13U)
#define IND_A2DP_N      (14U)
#define PRGM_SENSE_N    (17U)
#define DC_OUT          ( A1)

#define MSGEQ7_INPUT_PULLUP_ON_DC_OUT (false)
#define MSGEQ7_SIGNAL_BAND_COUNT      ((uint8_t)7U)
#define MSGEQ7_ZERO_SIGNAL_LEVEL      ((uint16_t)400U)

// create BM62 driver object
BM62::BM62 bluetooth(PRGM_SENSE_N, RST_N, IND_A2DP_N, &Serial);

// create MSGEQ7 driver object
MSGEQ7::MSGEQ7 spectrum(STROBE, DC_OUT, RESET, MSGEQ7_INPUT_PULLUP_ON_DC_OUT);

// create MAX9744 driver object
MAX9744::MAX9744 amplifier(MAX9744_DEFAULT_I2CADDR, MUTE, SHDN, &Wire);

// create DS1882 driver object
DS1882::DS1882 digipot(DS1882_DEFAULT_I2CADDR, DS1882_EN_N, &Wire);

// create CS4270 driver object
CS4270::CS4270 codec70(CS4270_DEFAULT_I2CADDR, CS4270_RST_N, &Wire);

// create AK5558 driver object
AK5558::AK5558 ADC58(AK5558_DEFAULT_I2CADDR, AK5558_RESET_L, &Wire);

// create PCAL6308A driver object
PCAL6408A::PCAL6408A Gpio(PCAL6408A_DEFAULT_I2CADDR, PCAL6408A_RST_L, PCAL6408A_INT_L, &Wire);

// create LED and LED+button objects for S1 and S2 user interface switches
ButtonLED::LED led(LED_PIN);
ButtonLED::ButtonLED ledbutton(BUTTON_PIN, led);

void setup() {
  // initialize serial console and set baud rate
  Serial.setTimeout(SERIAL_TIMEOUT_MILLISECONDS);
  Serial.begin(SERIAL_BAUD_RATE);

  // initialize Wire library and set clock rate to 400 kHz
  Wire.begin();
  Wire.setClock(I2C_CLOCK_RATE);

  // example driver initialization goes here
  bluetooth.init();
  spectrum.init();
  amplifier.init();
  digipot.init();
  codec70.init();
  ADC58.init();
  !Gpio.init();
  ledbutton.init();
}

void loop() {
    // main code goes here
}
```
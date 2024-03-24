#include <Wire.h>

#include "audio/audiomath.h"
#include "audio/audiomath.cpp"

#include "audio/CS4270.h"
#include "audio/CS4270.cpp"

#include "audio/DS1882.h"
#include "audio/DS1882.cpp"

#include "audio/MAX9744.h"
#include "audio/MAX9744.cpp"

#include "audio/MSGEQ7.h"
#include "audio/MSGEQ7.cpp"

#include "bluetooth/BM62.h"
#include "bluetooth/BM62.cpp"

#include "io/buttonled.h"
#include "io/buttonled.cpp"

#include "memory/storage.h"

#define SERIAL_TIMEOUT_MILLISECONDS (1000U)
#define SERIAL_BAUD_RATE (115200U)
#define I2C_CLOCK_RATE   (400000U)

// define the digitalOut pins according to the schematic net name
#define BUTTON_PIN    ((uint8_t)3U)
#define IND_A2DP_N    ((uint8_t)4U)
#define LED_PIN       ((uint8_t)5U)
#define DS1882_EN_N   ((uint8_t)5U)
#define RST_N         ((uint8_t)8U)
#define STROBE        ((uint8_t)9U)
#define RESET         ((uint8_t)10U)
#define CS4270_RST_N  ((uint8_t)11U)
#define MUTE          ((uint8_t)12U)
#define SHDN          ((uint8_t)13U)
#define PRGM_SENSE_N  ((uint8_t)17U)
#define DC_OUT        (A1)

#define MSGEQ7_INPUT_PULLUP_ON_DC_OUT (false)
#define MSGEQ7_SIGNAL_BAND_COUNT      ((uint8_t)7U)
#define MSGEQ7_ZERO_SIGNAL_LEVEL      ((uint16_t)400U)

// create BM62 driver object
BM62 bluetooth(PRGM_SENSE_N, RST_N, IND_A2DP_N, &Serial);

// create MSGEQ7 driver object
MSGEQ7 spectrum(STROBE, DC_OUT, RESET, MSGEQ7_INPUT_PULLUP_ON_DC_OUT);

// create MAX9744 driver object
MAX9744 amplifier(MAX9744_DEFAULT_I2CADDR, MUTE, SHDN, &Wire);

// create DS1882 driver object
DS1882 digipot(DS1882_DEFAULT_I2CADDR, DS1882_EN_N, &Wire);

// create CS4270 driver object
CS4270 codec70(CS4270_DEFAULT_I2CADDR, CS4270_RST_N, &Wire);

// create LED and LED+button objects for S1 and S2 user interface switches
LED led(LED_PIN);
ButtonLED ledbutton(BUTTON_PIN, led);

/*! @brief  Called once prior to loop() following power-on or reset
 *
 * @details Detailed description here.
 * @todo    Bring in improvements from motor-control project
 */
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
  ledbutton.init();
}

/*! @brief  An infinite for(;;) loop within Arduino's main()
 *
 * @details Detailed description here.
 * @todo    Bring in improvements from motor-control project
 */
void loop() {

}
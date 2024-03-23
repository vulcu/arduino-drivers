#include <Wire.h>

#include "audio/audiomath.h"
#include "audio/CS4270.h"
#include "audio/DS1882.h"
#include "audio/MAX9744.h"
#include "audio/MSGEQ7.h"
#include "bluetooth/BM62.h"
#include "io/ledbutton.h"

#define SERIAL_TIMEOUT_MILLISECONDS (1000U)
#define SERIAL_BAUD_RATE (115200U)
#define I2C_CLOCK_RATE   (400000U)

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

  // remaining board configuration code goes here
}


/*! @brief  An infinite for(;;) loop within Arduino's main()
 *
 * @details Detailed description here.
 * @todo    Bring in improvements from motor-control project
 */
void loop() {

}
#include <Wire.h>

#include "buttonled.h"


// define the digital I/O for various drivers
#define BUTTON_PIN      ( 3U)
#define LED_PIN         ( 5U)

// create LED and LED+button objects for a user interface
ButtonLED::LED led(LED_PIN);
ButtonLED::ButtonLED ledbutton(BUTTON_PIN, led);

void setup() {
  // example driver initialization goes here
  ledbutton.init();
}

void loop() {
    // main code goes here
}
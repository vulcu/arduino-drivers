/*
 * buttonled.cpp - UI Push-button Driver w/ LED Support for Arduino
 * Copyright (c) 2017 Winry R. Litwa-Vulcu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "buttonled.h"

namespace ButtonLED {
  // class constructor for LED object
  LED::LED(int8_t led_pin) :
    led_pin(led_pin) { }

  // attach the LED to a hardware pin
  void LED::attach(const int8_t led_pin) {
    this->led_pin = led_pin;
    this->init();
  }

  // write LED PWM value (0-255)
  void LED::brightness(const uint8_t value) {
    if (led_pin >= 0 ) {
        // don't write anything if no valid LED pin is defined
        analogWrite(led_pin, value);
    }
  }

  // configure the port and set initial LED state to OFF
  void LED::init(void) {
    pinMode(led_pin, OUTPUT);
    this->off();
  }

  // set the LED to OFF
  void LED::off(void) {
    if (led_pin >= 0 ) {
      // don't write anything if no valid LED pin is defined
      digitalWrite(led_pin, LOW);
    }
  }

  // set the LED to ON
  void LED::on(void) {
    if (led_pin >= 0 ) {
      // don't write anything if no valid LED pin is defined
      digitalWrite(led_pin, HIGH);
    }
  }

  // turn the LED off and clear the pin attachment
  void LED::reset(void) {
    this->off();
    this->led_pin = -1;
  }

  /**************************************************************************/
  // class constructor for Button object
  Button::Button(uint8_t button_pin) :
    button_pin(button_pin),
    button_port_input_mode(INPUT) { }

  // disable the input pullup for the attached pin
  void Button::disableInputPullup(void) {
    button_port_input_mode = INPUT;
    this->init();
  }

  // enable the input pullup for the attached pin
  void Button::enableInputPullup(void) {
    button_port_input_mode = INPUT_PULLUP;
    this->init();
  }

  void Button::init(void) {
    // configure the port and input pullup mode
    pinMode(button_pin, button_port_input_mode);
  }

  // read the instantaneous value of the attached Button
  bool Button::read(void) {
    return (bool)digitalRead(button_pin);
  }

  /**************************************************************************/
  // class constructor for ButtonLED object
  ButtonLED::ButtonLED(int8_t button_pin, LED &led) :
    Button(button_pin),
    led(led) { }

  // attach an LED pin and configure the LED for use
  void ButtonLED::attach(const int8_t led_pin) {
    led.attach(led_pin);
  }

  // write a PWM brightness to the Button LED
  void ButtonLED::brightness(const uint8_t value) {
    led.brightness(value);
  }

  // disable the input pullup for the Button input
  void ButtonLED::disableInputPullup(void) {
    Button::disableInputPullup();
    led.init();
  }

  // enable the input pullup for the Button input
  void ButtonLED::enableInputPullup(void) {
    Button::enableInputPullup();
    led.init();
  }

  // initialize the hardware button and LED
  void ButtonLED::init(void) {
    Button::init();
    led.init();
  }

  // set the Button LED to OFF
  void ButtonLED::off(void) {
    led.off();
  }

  // reset the button's LED pin attachment (disables control of LED)
  void ButtonLED::unattach(void) {
    led.reset();
  }
}

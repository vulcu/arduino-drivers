/*
 * PCAL6408A.cpp - NXP PCAL6408A IO Expander driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "PCAL6408A.h"

#ifndef PCAL6408A_REGISTER_BIT_FIELD_NAMES

  #define PCAL6408A_REGISTER_BIT_FIELD_NAMES

#endif

// PCAL6408A default register values for initialization
const int16_t default_config[12] PROGMEM = 
{
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00,                 //Test Register (must be 0x00)
  0x00                  //Test Register (must be 0x00)
};

PCAL6408A::PCAL6408A(uint8_t i2c_address, uint8_t reset_n, uint8_t interrupt_n, TwoWire *Wire) : 
                     i2c_address(i2c_address), reset_n(reset_n), interrupt_n(interrupt_n) {
  this->Wire = Wire;
  resetActiveConfig();
};


void PCAL6408A::init(void) {
  // set AK5558 PDN pin HIGH to put in reset
  shutdown();

  resetActiveConfig();

  // set AK5558 PDN pin Low to take out of reset
  enable();
}


void PCAL6408A::enable(void) {
  pinMode(reset_n, OUTPUT);
  digitalWrite(reset_n, LOW);
}


void PCAL6408A::shutdown(void) {
  pinMode(reset_n, OUTPUT);
  digitalWrite(reset_n, HIGH);
}

void PCAL6408A::resetActiveConfig(void) {
  memcpy_P(active_config, default_config, sizeof(default_config));
}

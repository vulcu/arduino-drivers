/*
 * PCAL6408A.cpp - NXP PCAL6408A IO Expander driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "PCAL6408A.h"

namespace NXP {
  using namespace PCAL6408ATypes;

  // PCAL6408A default register values for initialization
  static const uint8_t PCAL6408A::default_config[12] PROGMEM = 
  {
    0x00,                 // Input Port
    0xFF,                 // Output Port
    0x00,                 // Polarity Inversion
    0xFF,                 // Configuration
    0xFF,                 // Output Drive Strength 0
    0xFF,                 // Output Drive Strength 1
    0x00,                 // Input Latch
    0x00,                 // Pull-up/Pull-down Enable
    0xFF,                 // Pull-up/Pull-down Selection
    0xFF,                 // Interrupt Mask
    0x00,                 // Interrupt Status
    0x00                  // Output Port Configuration
  };

  PCAL6408A::PCAL6408A(uint8_t i2c_address, uint8_t reset_n, uint8_t interrupt_n, TwoWire *pWire) : 
                      i2c_address(i2c_address), reset_n(reset_n), interrupt_n(interrupt_n) {
    this->pWire = pWire;
    resetActiveConfig();
  };

  void PCAL6408A::init(void) {
    // set PCAL6408A RESET pin HIGH to put in reset
    shutdown();
    resetActiveConfig();

    // configure output port
    pWire->beginTransmission(i2c_address);
      pWire->write(OUTPUT_PORT_PTR);
      pWire->write(pgm_read_byte(&(default_config[OUTPUT_PORT])));
    pWire->endTransmission();

    // configure polarity inversion
    pWire->beginTransmission(i2c_address);
      pWire->write(POLARITY_INVERSION_PTR);
      pWire->write(pgm_read_byte(&(default_config[POLARITY_INVERSION])));
    pWire->endTransmission();

    // configure pin I/O direction
    pWire->beginTransmission(i2c_address);
      pWire->write(CONFIGURATION_PTR);
      pWire->write(pgm_read_byte(&(default_config[CONFIGURATION])));
    pWire->endTransmission();

    // configure output drive strength
    pWire->beginTransmission(i2c_address);
      pWire->write(DRIVE_STRENGTH_0_PTR);
      pWire->write(pgm_read_byte(&(default_config[DRIVE_STRENGTH_0])));
    pWire->endTransmission();
    pWire->beginTransmission(i2c_address);
      pWire->write(DRIVE_STRENGTH_1_PTR);
      pWire->write(pgm_read_byte(&(default_config[DRIVE_STRENGTH_1])));
    pWire->endTransmission();

    // configure input port latching
    pWire->beginTransmission(i2c_address);
      pWire->write(INPUT_LATCH_PTR);
      pWire->write(pgm_read_byte(&(default_config[INPUT_LATCH])));
    pWire->endTransmission();

    // configure pull-up/pull-down enable
    pWire->beginTransmission(i2c_address);
      pWire->write(PULLUP_PULLDOWN_EN_PTR);
      pWire->write(pgm_read_byte(&(default_config[PULLUP_PULLDOWN_EN])));
    pWire->endTransmission();

    // configure pull-up/pull-down selection
    pWire->beginTransmission(i2c_address);
      pWire->write(PULLUP_PULLDOWN_SEL_PTR);
      pWire->write(pgm_read_byte(&(default_config[PULLUP_PULLDOWN_SEL])));
    pWire->endTransmission();

    // configure interrupt mask
    pWire->beginTransmission(i2c_address);
      pWire->write(INTERRUPT_MASK_PTR);
      pWire->write(pgm_read_byte(&(default_config[INTERRUPT_MASK])));
    pWire->endTransmission();

    // configure input port
    pWire->beginTransmission(i2c_address);
      pWire->write(OUTPUT_PORT_CONFIG_PTR);
      pWire->write(pgm_read_byte(&(default_config[OUTPUT_PORT_CONFIG])));
    pWire->endTransmission();

    // set PCAL6408A RESET pin Low to take out of reset
    enable();
  }

  void PCAL6408A::enable(void) {
    pinMode(reset_n, OUTPUT);
    digitalWrite(reset_n, HIGH);
  }

  bool PCAL6408A::get(register_bitmask_t input_port) {
    // read logical state of specified port bit
    pWire->beginTransmission(i2c_address);
    pWire->write(INPUT_PORT);
    pWire->endTransmission();
    delayMicroseconds(50);
    pWire->requestFrom(i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)pWire->read();
    pWire->endTransmission();

    return (bool)((read_data & input_port) >> input_port);
  }

  void PCAL6408A::set(register_bitmask_t output_port, bool value) {
    // read logical state of specified port first to avoid overwriting data
    pWire->beginTransmission(i2c_address);
    pWire->write(OUTPUT_PORT);
    pWire->endTransmission();
    delayMicroseconds(50);
    pWire->requestFrom(i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)pWire->read();
    pWire->endTransmission();

    // write logical state of specified port bit
    pWire->beginTransmission(i2c_address);
      pWire->write(OUTPUT_PORT);
      if (value = HIGH) {
        pWire->write((read_data | output_port));
      }
      else {
        pWire->write((read_data & ~output_port));
      }
    pWire->endTransmission();
  }

  void PCAL6408A::shutdown(void) {
    pinMode(reset_n, OUTPUT);
    digitalWrite(reset_n, LOW);
  }

  void PCAL6408A::resetActiveConfig(void) {
    memcpy_P(active_config, default_config, sizeof(default_config));
  }
}

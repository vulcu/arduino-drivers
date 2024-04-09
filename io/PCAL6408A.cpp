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
    i2c_address(i2c_address), 
    reset_n(reset_n), 
    interrupt_n(interrupt_n) {
    this->pWire = pWire;
    this->resetActiveConfig();
  };

  void PCAL6408A::init(void) {
    // set PCAL6408A RESET pin HIGH to take out of reset
    this->shutdown();
    delayMicroseconds(100);
    this->enable();
    delayMicroseconds(100);

    // configure output port
    pWire->beginTransmission(this->i2c_address);
      pWire->write(OUTPUT_PORT_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[OUTPUT_PORT])));
    pWire->endTransmission();

    // configure polarity inversion
    pWire->beginTransmission(this->i2c_address);
      pWire->write(POLARITY_INVERSION_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[POLARITY_INVERSION])));
    pWire->endTransmission();

    // configure pin I/O direction
    pWire->beginTransmission(this->i2c_address);
      pWire->write(CONFIGURATION_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[CONFIGURATION])));
    pWire->endTransmission();

    /// TODO: Register writes to 0x40 and up are NACK unless register 0x03 is
    /// configured to OUTPUT... but even if it is, writes don't work every time?
    // configure output drive strength
    pWire->beginTransmission(this->i2c_address);
      pWire->write(DRIVE_STRENGTH_0_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[DRIVE_STRENGTH_0])));
    pWire->endTransmission();
    pWire->beginTransmission(this->i2c_address);
      pWire->write(DRIVE_STRENGTH_1_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[DRIVE_STRENGTH_1])));
    pWire->endTransmission();

    // configure input port latching
    pWire->beginTransmission(this->i2c_address);
      pWire->write(INPUT_LATCH_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[INPUT_LATCH])));
    pWire->endTransmission();

    // configure pull-up/pull-down enable
    pWire->beginTransmission(this->i2c_address);
      pWire->write(PULLUP_PULLDOWN_EN_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[PULLUP_PULLDOWN_EN])));
    pWire->endTransmission();

    // configure pull-up/pull-down selection
    pWire->beginTransmission(this->i2c_address);
      pWire->write(PULLUP_PULLDOWN_SEL_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[PULLUP_PULLDOWN_SEL])));
    pWire->endTransmission();

    // configure interrupt mask
    pWire->beginTransmission(this->i2c_address);
      pWire->write(INTERRUPT_MASK_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[INTERRUPT_MASK])));
    pWire->endTransmission();

    // configure input port
    pWire->beginTransmission(this->i2c_address);
      pWire->write(OUTPUT_PORT_CONFIG_PTR);
      pWire->write(pgm_read_byte(&(this->default_config[OUTPUT_PORT_CONFIG])));
    pWire->endTransmission();

    this->resetActiveConfig();
  }

  void PCAL6408A::enable(void) {
    pinMode(this->reset_n, OUTPUT);
    digitalWrite(this->reset_n, HIGH);
  }

  uint8_t PCAL6408A::readInput(void) {
    return this->getRegister(INPUT_PORT_PTR);
  }

  bool PCAL6408A::readInputPin(register_bitmask_t port_pin) {
    return this->getRegisterBit(INPUT_PORT_PTR, port_pin);
  }

  void PCAL6408A::writeOutput(uint8_t value) {
    this->setRegister(OUTPUT_PORT_PTR, value);
  }

  void PCAL6408A::writeOutputPin(register_bitmask_t port_pin, bool value) {
    if (this->getRegisterBit(CONFIGURATION_PTR, port_pin)) {
      this->setRegisterBit(CONFIGURATION_PTR, port_pin, false);
    }
    this->setRegister(OUTPUT_PORT_PTR, value);
  }

  void PCAL6408A::setPortPinAsInput(register_bitmask_t port_pin) {
    this->setRegisterBit(CONFIGURATION_PTR, port_pin, true);
  }

  void PCAL6408A::setAllPinsAsInput(void) {
    this->setRegister(CONFIGURATION_PTR, 0xFF);
  }

  void PCAL6408A::setPortPinAsOutput(register_bitmask_t port_pin) {
    this->setRegisterBit(CONFIGURATION_PTR, port_pin, false);
  }

  void PCAL6408A::setAllPinsAsOutput(void) {
    this->setRegister(CONFIGURATION_PTR, 0x00);
  }

  void PCAL6408A::setPortPinPolarity(register_bitmask_t port_pin, polarity_inversion_t polarity) {
    this->setRegisterBit(POLARITY_INVERSION_PTR, port_pin, (bool)polarity);
  }

  void PCAL6408A::setAllPinsPolarity(polarity_inversion_t polarity) {
    if (polarity == INVERTED) {
      this->setRegister(POLARITY_INVERSION_PTR, 0xFF);
    }
    else {
      this->setRegister(POLARITY_INVERSION_PTR, 0x00);
    }
  }

  uint8_t PCAL6408A::getRegister(register_pointer_t register_pointer) {
    // read logical state of specified port bit
    pWire->beginTransmission(this->i2c_address);
    pWire->write(register_pointer);
    pWire->endTransmission();
    delayMicroseconds(50);
    pWire->requestFrom(this->i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)pWire->read();
    pWire->endTransmission();

    return read_data;
  }

  bool PCAL6408A::getRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask) {
    // read logical state of specified port bit
    pWire->beginTransmission(this->i2c_address);
    pWire->write(register_pointer);
    pWire->endTransmission();
    delayMicroseconds(50);
    pWire->requestFrom(this->i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)pWire->read();
    pWire->endTransmission();

    return (bool)((read_data & bitmask) >> bitmask);
  }

  void PCAL6408A::setRegister(register_pointer_t register_pointer, uint8_t value) {
    // write logical state of entire specified register
    pWire->beginTransmission(this->i2c_address);
      pWire->write(register_pointer);
      pWire->write(value);
    pWire->endTransmission();
  }

  void PCAL6408A::setRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask, bool value) {
    // read logical state of specified port first to avoid overwriting data
    uint8_t read_data = this->getRegister(register_pointer);

    // write logical state of specified port bit
    pWire->beginTransmission(this->i2c_address);
      pWire->write(register_pointer);
      if (value = HIGH) {
        pWire->write((read_data | bitmask));
      }
      else {
        pWire->write((read_data & ~bitmask));
      }
    pWire->endTransmission();
  }

  void PCAL6408A::shutdown(void) {
    pinMode(this->reset_n, OUTPUT);
    digitalWrite(this->reset_n, LOW);
  }

  void PCAL6408A::resetActiveConfig(void) {
    memcpy_P(this->active_config, this->default_config, sizeof(this->default_config));
  }
}

/*
 * AK5558.cpp - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "AK5558.h"

namespace AK5558 {
  using namespace AK5558Types;

  // AK5558 default register values for initialization
  // Values are selected to work with MCHStreamer TDM I/O
  const uint8_t AK5558::default_config[8] PROGMEM = 
  {
    0xFF,                 // Power Management
    0x01,                 // Channel Summing and Timing Reset
    0x35,                 // Clocking, DAI Mode, HP Filter
    0x40,                 // TDM Mode Selection
    0x00,                 // LP Filter, DSD Mode
    0x00,                 // DSD Configuration
    0x00,                 // Test Register (must be 0x00)
    0x00                  // Test Register (must be 0x00)
  };


  AK5558::AK5558(uint8_t i2c_address, uint8_t reset_n, TwoWire *pWire) :
    i2c_address(i2c_address), 
    reset_n(reset_n),
    active_config{0x00} {
    this->reset();
    this->pWire = pWire;
    this->resetActiveConfig();
  }

  /// TODO: make audio IO configurable during init instead of defaulting to TDM256
  bool AK5558::init(void) {
    // delay AK5558 enable to ensure correct initialization (datasheet pp.55-56)
    this->reset();
    delayMicroseconds(100);
    this->enable();
    delayMicroseconds(AK5558_INT_PDN_OSCCLK_DELAY_MICROS);

    // set power management for channels 1-8 to OFF state (0)
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(PWRMGMT1);
      this->pWire->write(0x00);
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();
    
    // use the first TwoWire transaction during init to check if communication is working
    if (error == NACK_ADDRESS) {
      return false;
    }

    // configure channel summing and timing reset
    this->writeDefaultConfigToRegister(PWRMGMT2);

    // configure clocking, DAI mode, HP filter
    this->writeDefaultConfigToRegister(CONTROL1);

    // configure TDM mode selection
    this->writeDefaultConfigToRegister(CONTROL2);

    // configure LP filter and DSD mode
    this->writeDefaultConfigToRegister(CONTROL3);

    //  configure DSD frequency, phase, clocking
    this->writeDefaultConfigToRegister(DSD);

    // configure power management state for channels 1-8
    this->writeDefaultConfigToRegister(PWRMGMT1);

    // set the active configuration to default configuration values
    this->resetActiveConfig();

    // initialization was successful
    return true;
  }

  void AK5558::enable(void) {
    if (!digitalRead(reset_n)) {
      pinMode(this->reset_n, OUTPUT);
      digitalWrite(this->reset_n, HIGH);
    }
    this->setRegisterBit(PWRMGMT2, register_bm(RSTN_BM), HIGH);
  }

  void AK5558::shutdown(void) {
    this->setRegisterBit(PWRMGMT2, register_bm(RSTN_BM), LOW);
  }

  void AK5558::reset(void) {
    pinMode(this->reset_n, OUTPUT);
    digitalWrite(this->reset_n, LOW);
  }

  void AK5558::mute(channel_select_t channel) {
    
  }

  void AK5558::unmute(channel_select_t channel) {
    
  }

  uint8_t AK5558::getRegister(register_pointer_t register_pointer) {
    // read logical state of specified port bit
    this->pWire->beginTransmission(this->i2c_address);
    this->pWire->write(register_pointer);
    this->pWire->endTransmission();
    delayMicroseconds(50);
    this->pWire->requestFrom(this->i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)this->pWire->read();

    return read_data;
  }

  bool AK5558::getRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask) {
    // read logical state of specified port bit
    this->pWire->beginTransmission(this->i2c_address);
    this->pWire->write(register_pointer);
    this->pWire->endTransmission();
    delayMicroseconds(50);
    this->pWire->requestFrom(this->i2c_address, 1U);
    delayMicroseconds(50);
    uint8_t read_data = (uint8_t)this->pWire->read();

    return (bool)((read_data & bitmask) >> bitmask);
  }

  void AK5558::setRegister(register_pointer_t register_pointer, uint8_t value) {
    // write logical state of entire specified register
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(register_pointer);
      this->pWire->write(value);
    this->pWire->endTransmission();
  }

  void AK5558::setRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask, bool value) {
    // read logical state of specified port first to avoid overwriting data
    uint8_t read_data = this->getRegister(register_pointer);

    // write logical state of specified port bit
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(register_pointer);
      if (value == HIGH) {
        this->pWire->write((read_data | bitmask));
      }
      else {
        this->pWire->write((read_data & ~bitmask));
      }
    this->pWire->endTransmission();
  }

  void AK5558::writeDefaultConfigToRegister(register_pointer_t register_pointer) {
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(register_pointer);
      this->pWire->write(pgm_read_byte(&(this->default_config[register_pointer])));
    this->pWire->endTransmission();
  }

  void AK5558::resetActiveConfig(void) {
    memcpy_P(this->active_config, this->default_config, sizeof(this->default_config));
  }
}

/*
 * AK5558.cpp - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "AK5558.h"

namespace AKM {
  using namespace AK5558Types;

  // AK5558 default register values for initialization
  static const uint8_t AK5558::default_config[8] PROGMEM = 
  {
    0xFF,                 // Power Management
    0x01,                 // Channel Summing and Timing Reset
    0x01,                 // Clocking, DAI Mode, HP Filter
    0x00,                 // TDM Mode Selection
    0x00,                 // LP Filter, DSD Mode
    0x00,                 // DSD Configuration
    0x00,                 // Test Register (must be 0x00)
    0x00                  // Test Register (must be 0x00)
  };


  AK5558::AK5558(uint8_t i2c_address, uint8_t reset_n, TwoWire *pWire) :
                invert_mute(false), i2c_address(i2c_address), reset_n(reset_n) {
    this->pWire = pWire;
    resetActiveConfig();
  }

  void AK5558::init(void) {
    // set AK5558 PDN pin HIGH to put in reset
    shutdown();

    // configure power management
    pWire->beginTransmission(i2c_address);
      pWire->write(PWRMGMT1);
      pWire->write(pgm_read_byte(&(default_config[PWRMGMT1])));
    pWire->endTransmission();

    // configure channel summing and timing reset
    pWire->beginTransmission(i2c_address);
      pWire->write(PWRMGMT2);
      pWire->write(pgm_read_byte(&(default_config[PWRMGMT2])));
    pWire->endTransmission();

    // configure clocking, DAI mode, HP filter
    pWire->beginTransmission(i2c_address);
      pWire->write(CONTROL1);
      pWire->write(pgm_read_byte(&(default_config[CONTROL1])));
    pWire->endTransmission();

    // configure TDM mode selection
    pWire->beginTransmission(i2c_address);
      pWire->write(CONTROL2);
      pWire->write(pgm_read_byte(&(default_config[CONTROL2])));
    pWire->endTransmission();

    // configure LP filter and DSD mode
    pWire->beginTransmission(i2c_address);
      pWire->write(CONTROL3);
      pWire->write(pgm_read_byte(&(default_config[CONTROL3])));
    pWire->endTransmission();

    //  configure DSD frequency, phase, clocking
    pWire->beginTransmission(i2c_address);
      pWire->write(DSD);
      pWire->write(pgm_read_byte(&(default_config[DSD])));
    pWire->endTransmission();

    //  configure Test registers (MUST be 0x00)
    pWire->beginTransmission(i2c_address);
      pWire->write(TEST1);
      pWire->write(pgm_read_byte(&(default_config[TEST1])));
    pWire->endTransmission();
    pWire->beginTransmission(i2c_address);
      pWire->write(TEST2);
      pWire->write(pgm_read_byte(&(default_config[TEST2])));
    pWire->endTransmission();

    resetActiveConfig();

    // set AK5558 PDN pin Low to take out of reset
    enable();
  }

  void AK5558::enable(void) {
    pinMode(reset_n, OUTPUT);
    digitalWrite(reset_n, LOW);
  }

  void AK5558::shutdown(void) {
    pinMode(reset_n, OUTPUT);
    digitalWrite(reset_n, HIGH);
  }

  void AK5558::invertMuteLogic(const bool invert_mute) {
    this->invert_mute = invert_mute;
  }

  void AK5558::mute(void) {
    // if the mute signal is inverted then clear control bit to mute
    if (invert_mute) {
      // TODO: mute device by setting bit LOW
    }
    else {
      // TODO: mute device by setting bit HIGH
    }
  }

  void AK5558::unmute(void) {
    // if the mute signal is inverted then set control bit to unmute
    if (invert_mute) {
      // TODO: unmute device by setting bit HIGH
    }
    else {
      // TODO: unmute device by setting bit LOW
    }
  }

  void AK5558::resetActiveConfig(void) {
    memcpy_P(active_config, default_config, sizeof(default_config));
  }
}

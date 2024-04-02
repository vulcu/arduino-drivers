/*
 * AK5558.cpp - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "AK5558.h"

#ifndef AK5558_REGISTER_BIT_FIELD_NAMES

  #define AK5558_REGISTER_BIT_FIELD_NAMES

  #define PW1_BM     (0x01 << 0)
  #define PW2_BM     (0x01 << 1)
  #define PW3_BM     (0x01 << 2)
  #define PW4_BM     (0x01 << 3)
  #define PW5_BM     (0x01 << 4)
  #define PW6_BM     (0x01 << 5)
  #define PW7_BM     (0x01 << 6)
  #define PW8_BM     (0x01 << 7)

  #define RSTN_BM    (0x01 << 0)
  #define MONO1_BM   (0x01 << 1)
  #define MONO2_BM   (0x01 << 2)

  #define HFPE_BM    (0x01 << 0)
  #define DIF0_BM    (0x01 << 1)
  #define DIF1_BM    (0x01 << 2)
  #define CKS0_BM    (0x01 << 3)
  #define CKS1_BM    (0x01 << 4)
  #define CKS2_BM    (0x01 << 5)
  #define CKS3_BM    (0x01 << 6)

  #define TDM0_BM    (0x01 << 5)
  #define TDM1_BM    (0x01 << 6)

  #define SLOW_BM    (0x01 << 0)
  #define SD_BM      (0x01 << 1)

  #define DSDSEL0_BM (0x01 << 0)
  #define DSDSEL1_BM (0x01 << 1)
  #define DCKB_BM    (0x01 << 2)
  #define PMOD_BM    (0x01 << 3)
  #define DCKS_BM    (0x01 << 5)

#endif

// AK5558 default register values for initialization
static const uint8_t default_config[8] PROGMEM = 
{
  0xFF,                 //Power Management
  0x01,                 //Channel Summing and Timing Reset
  0x01,                 //Clocking, DAI Mode, HP Filter
  0x00,                 //TDM Mode Selection
  0x00,                 //LP Filter, DSD Mode
  0x00,                 //DSD Configuration
  0x00,                 //Test Register (must be 0x00)
  0x00                  //Test Register (must be 0x00)
};


AK5558::AK5558(uint8_t i2c_address, uint8_t reset_n, TwoWire *Wire) :
               invert_mute(false), i2c_address(i2c_address), reset_n(reset_n) {
  this->Wire = Wire;
  resetActiveConfig();
}


void AK5558::init(void) {
  // set AK5558 PDN pin HIGH to put in reset
  shutdown();

  // configure power management
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_PWRMGMT1);
    Wire->write(pgm_read_byte(&(default_config[AK5558_PWRMGMT1])));
  Wire->endTransmission();

  // configure channel summing and timing reset
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_PWRMGMT2);
    Wire->write(pgm_read_byte(&(default_config[AK5558_PWRMGMT2])));
  Wire->endTransmission();

  // configure clocking, DAI mode, HP filter
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_CONTROL1);
    Wire->write(pgm_read_byte(&(default_config[AK5558_CONTROL1])));
  Wire->endTransmission();

  // configure TDM mode selection
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_CONTROL2);
    Wire->write(pgm_read_byte(&(default_config[AK5558_CONTROL2])));
  Wire->endTransmission();

  // configure LP filter and DSD mode
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_CONTROL3);
    Wire->write(pgm_read_byte(&(default_config[AK5558_CONTROL3])));
  Wire->endTransmission();

  //  configure DSD frequency, phase, clocking
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_DSD);
    Wire->write(pgm_read_byte(&(default_config[AK5558_DSD])));
  Wire->endTransmission();

  //  configure Test registers (MUST be 0x00)
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_TEST1);
    Wire->write(pgm_read_byte(&(default_config[AK5558_TEST1])));
  Wire->endTransmission();
  Wire->beginTransmission(i2c_address);
    Wire->write(AK5558_TEST2);
    Wire->write(pgm_read_byte(&(default_config[AK5558_TEST2])));
  Wire->endTransmission();

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

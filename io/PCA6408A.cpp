/*
 * PCA6408A.cpp - PCA6408A IO Expander driver for Arduino
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include "PCA6408A.h"

namespace PCA6408A {
  using namespace PCA6408ATypes;

  // PCA6408A default register values for initialization
  const uint8_t PCA6408A::default_config[12] PROGMEM = 
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

  PCA6408A::PCA6408A(uint8_t i2c_address, uint8_t reset_n, uint8_t interrupt_n, TwoWire *pWire) : 
    i2c_address(i2c_address), 
    reset_n(reset_n), 
    interrupt_n(interrupt_n),
    agile_io_available(false),
    active_config({0x00}) {
    this->shutdown();
    this->pWire = pWire;
    // this->resetActiveConfig();
  };

  bool PCA6408A::init(void) {
    // set PCA6408A RESET pin HIGH to take out of reset
    this->shutdown();
    delayMicroseconds(100);
    this->enable();
    delayMicroseconds(100);

    // configure output port
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(OUTPUT_PORT_PTR);
      this->pWire->write(pgm_read_byte(&(this->default_config[OUTPUT_PORT])));
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();
    
    // use the first TwoWire transaction during init to check if communication is working
    if (error == NACK_ADDRESS) {
      return false;
    }

    // configure polarity inversion
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(POLARITY_INVERSION_PTR);
      this->pWire->write(pgm_read_byte(&(this->default_config[POLARITY_INVERSION])));
    this->pWire->endTransmission();

    // configure pin I/O direction
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(CONFIGURATION_PTR);
      this->pWire->write(pgm_read_byte(&(this->default_config[CONFIGURATION])));
    this->pWire->endTransmission();

    // configure output drive strength [Port 0:3] (available iff IC is NXP PCAL6408A)
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(DRIVE_STRENGTH_0_PTR);
      this->pWire->write(pgm_read_byte(&(this->default_config[DRIVE_STRENGTH_0])));
    error = (twi_error_type_t)this->pWire->endTransmission();

    // If writing to drive strength register succeeds then IC is 
    // likely NXP PCAL6408A and contains Agile I/O feature set.
    // If writing to drive strength registers fails then IC is 
    // a software-compatible alternate e.g. PCA6408A or TCA6408A
    if (error == NO_ERROR) {
      this->agile_io_available = true;

      // configure output drive strength [Port 4:7] (available iff IC is NXP PCAL6408A)
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(DRIVE_STRENGTH_1_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[DRIVE_STRENGTH_1])));
      this->pWire->endTransmission();

      // configure input port latching
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(INPUT_LATCH_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[INPUT_LATCH])));
      this->pWire->endTransmission();

      // configure pull-up/pull-down enable
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(PULLUP_PULLDOWN_EN_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[PULLUP_PULLDOWN_EN])));
      this->pWire->endTransmission();

      // configure pull-up/pull-down selection
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(PULLUP_PULLDOWN_SEL_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[PULLUP_PULLDOWN_SEL])));
      this->pWire->endTransmission();

      // configure interrupt mask
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(INTERRUPT_MASK_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[INTERRUPT_MASK])));
      this->pWire->endTransmission();

      // configure input port
      this->pWire->beginTransmission(this->i2c_address);
        this->pWire->write(OUTPUT_PORT_CONFIG_PTR);
        this->pWire->write(pgm_read_byte(&(this->default_config[OUTPUT_PORT_CONFIG])));
      this->pWire->endTransmission();
    }

    this->resetActiveConfig();

    // initialization was successful
    return true;
  }

  void PCA6408A::enable(void) {
    pinMode(this->reset_n, OUTPUT);
    digitalWrite(this->reset_n, HIGH);
  }
  
  void PCA6408A::shutdown(void) {
    pinMode(this->reset_n, OUTPUT);
    digitalWrite(this->reset_n, LOW);
  }

  /// TODO: consider renaming this
  bool PCA6408A::isAgileIoAvailable(void) {
    return this->agile_io_available;
  }

  uint8_t PCA6408A::readInput(void) {
    return this->getRegister(INPUT_PORT_PTR);
  }

  bool PCA6408A::readInputPin(register_bitmask_t port_pin) {
    return this->getRegisterBit(INPUT_PORT_PTR, port_pin);
  }

  void PCA6408A::writeOutput(uint8_t value) {
    this->setRegister(OUTPUT_PORT_PTR, value);
  }

  void PCA6408A::writeOutputPin(register_bitmask_t port_pin, bool value) {
    if (this->getRegisterBit(CONFIGURATION_PTR, port_pin)) {
      this->setRegisterBit(CONFIGURATION_PTR, port_pin, false);
    }
    this->setRegister(OUTPUT_PORT_PTR, value);
  }

  void PCA6408A::setPortPinAsInput(register_bitmask_t port_pin) {
    this->setRegisterBit(CONFIGURATION_PTR, port_pin, true);
  }

  void PCA6408A::setAllPinsAsInput(void) {
    this->setRegister(CONFIGURATION_PTR, 0xFF);
  }

  void PCA6408A::setPortPinAsOutput(register_bitmask_t port_pin) {
    this->setRegisterBit(CONFIGURATION_PTR, port_pin, false);
  }

  void PCA6408A::setAllPinsAsOutput(void) {
    this->setRegister(CONFIGURATION_PTR, 0x00);
  }

  void PCA6408A::setPortPinPolarity(register_bitmask_t port_pin, polarity_inversion_t polarity) {
    this->setRegisterBit(POLARITY_INVERSION_PTR, port_pin, (bool)polarity);
  }

  void PCA6408A::setAllPinsPolarity(polarity_inversion_t polarity) {
    if (polarity == INVERTED) {
      this->setRegister(POLARITY_INVERSION_PTR, 0xFF);
    }
    else {
      this->setRegister(POLARITY_INVERSION_PTR, 0x00);
    }
  }

  uint8_t PCA6408A::getRegister(register_pointer_t register_pointer) {
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

  bool PCA6408A::getRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask) {
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

  void PCA6408A::setRegister(register_pointer_t register_pointer, uint8_t value) {
    // write logical state of entire specified register
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(register_pointer);
      this->pWire->write(value);
    this->pWire->endTransmission();
  }

  void PCA6408A::setRegisterBit(register_pointer_t register_pointer, register_bitmask_t bitmask, bool value) {
    // read logical state of specified port first to avoid overwriting data
    uint8_t read_data = this->getRegister(register_pointer);

    // write logical state of specified port bit
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(register_pointer);
      if (value = HIGH) {
        this->pWire->write((read_data | bitmask));
      }
      else {
        this->pWire->write((read_data & ~bitmask));
      }
    this->pWire->endTransmission();
  }

  void PCA6408A::resetActiveConfig(void) {
    memcpy_P(this->active_config, this->default_config, sizeof(this->default_config));
  }
}

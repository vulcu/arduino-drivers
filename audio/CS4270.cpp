/*
 * CS4270.cpp - Cirrus CS4270 Stereo CODEC for Arduino
 * Copyright (c) 2024 Winry R. Litwa-Vulcu. All rights reserved.
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

#include "CS4270.h"

namespace CS4270 {
  using namespace CS4270Types;

  // class constructor for CS4270 object
  CS4270::CS4270(uint8_t i2c_address, uint8_t enable_n, TwoWire* pWire) : 
    i2c_address(i2c_address), 
    reset_n(enable_n), 
    channel_attenuation({0U}) {
    this->pWire = pWire;
  }

  // initialize and configure the device
  bool CS4270::init(void) {
    // toggle CS4270 reset line to ensure correct initialization
    digitalWrite(reset_n, LOW);
    delayMicroseconds(100);
    digitalWrite(reset_n, HIGH);
    delayMicroseconds(100);
    digitalWrite(reset_n, LOW);
    delayMicroseconds(100);

    // set PDN bit to put CS4270 in standby
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_PWR_CTRL);
      this->pWire->write(0x01);
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();
      
    // use the first TwoWire transaction during init to check if communication is working
    if (error == NACK_ADDRESS) {
      return false;
    }

    // configure Mode Control register
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_MDE_CTRL);
      this->pWire->write(0x01);
    this->pWire->endTransmission();

    // configure ADC and DAC control
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_ADDACTRL);
      if (CS4270_LOOPBACK) {
        this->pWire->write(0x29);
      }
      else {
        this->pWire->write(0x09);
      }
    this->pWire->endTransmission();

    // configure Transition Control register
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_TRN_CTRL);
      if (CS4270_DEEMPHAS) {
        this->pWire->write(0xF1);
      }
      else {
        this->pWire->write(0xF0);
      }
    this->pWire->endTransmission();

    // configure Mute Control register
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_MTE_CTRL);
      if (CS4270_AUTOMUTE) {
        this->pWire->write(0x20);
      }
      else {
        this->pWire->write(0x00);
      }
    this->pWire->endTransmission();

    // set DAC Channel A volume control
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_DAC_VOLA);
      this->pWire->write(0x00);
    this->pWire->endTransmission();

    // set DAC Channel B volume control
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_DAC_VOLB);
      this->pWire->write(0x00);
    this->pWire->endTransmission();

    // clear PDN bit to take CS4270 out of standby
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_PWR_CTRL);
      this->pWire->write(0x00);
    this->pWire->endTransmission();

    // initialization was successful
    return true;
  }

  // enable the CS4270 by taking it out of shutdown (HIGH)
  void CS4270::enable(void) {
    digitalWrite(reset_n, HIGH);
  }

  // disable the CS4270 via the GPIO shutdown signal
  void CS4270::shutdown(void) {
    digitalWrite(reset_n, LOW);
  }

  // mute the CS4270 output by setting maximum attenuation
  void CS4270::mute(void) {
    // Mute Control register for both ADC and DAC
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_MTE_CTRL);
      this->pWire->write(0x1A);
    this->pWire->endTransmission();
  }

  // unmute the CS4270 amplifier by restoring previous attenuation
  void CS4270::unmute(void) {
    // Mute Control register for both ADC and DAC
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(CS4270_MTE_CTRL);
      this->pWire->write(0x00);
    this->pWire->endTransmission();
  }

  // set the potentiometers to a value between 0 [min] and 63 [max]
  void CS4270::volume(uint8_t value, channels_t channel) {
    // if the value and channel are within allowable range
    if ((value >= CS4270_MINIMUM_ATTENUATION) &&
        (value <= CS4270_MAXIMUM_ATTENUATION)) {
      
      switch (channel) {
        case Mono_ChA: {
          channel_attenuation[0] = value;

          //  set DAC Channel A volume control
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(CS4270_DAC_VOLA);
            this->pWire->write(0x00);
          this->pWire->endTransmission();
        }
        break;

        case Mono_ChB: {
          channel_attenuation[1] = value;

          //  set DAC Channel B volume control
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(CS4270_DAC_VOLB);
            this->pWire->write(0x00);
          this->pWire->endTransmission();
        }
        break;

        case Stereo: {
          memset(channel_attenuation, value, sizeof(channel_attenuation));

          //  set DAC Channel A volume control
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(CS4270_DAC_VOLA);
            this->pWire->write(0x00);
          this->pWire->endTransmission();

          //  set DAC Channel B volume control
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(CS4270_DAC_VOLB);
            this->pWire->write(0x00);
          this->pWire->endTransmission();
        }
        break;
      }
    }
  }
}

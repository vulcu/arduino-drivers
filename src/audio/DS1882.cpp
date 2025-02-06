/*
 * DS1882.cpp - Maxim DS1882 Digitally Controlle Potetiometer for Arduino
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

#include "DS1882.h"

namespace DS1882 {
  using namespace DS1882Types;

  // class constructor for DS1882 object
  DS1882::DS1882(uint8_t i2c_address, uint8_t enable_n, TwoWire* pWire) : 
    i2c_address(i2c_address), 
    enable_n(enable_n), 
    channel_attenuation{0U} {
    pinMode(this->enable_n, OUTPUT);
    this->shutdown();
    this->pWire = pWire;
  }

  // initialize and configure the device
  bool DS1882::init(void) {
    // toggle DS1882 reset line to ensure correct initialization
    digitalWrite(this->enable_n, LOW);
    delayMicroseconds(100);
    digitalWrite(this->enable_n, HIGH);
    delayMicroseconds(100);
    digitalWrite(this->enable_n, LOW);
    delayMicroseconds(100);

    
    
    this->pWire->beginTransmission(this->i2c_address);
      this->pWire->write(DS1882_CONFIGURATION);             // Configuration
      this->pWire->write(0x00 + DS1882_MINIMUM_VOL_LEVEL);  // Potentiometer 0
      this->pWire->write(0x40 + DS1882_MINIMUM_VOL_LEVEL);  // Potentiometer 1
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();

    // use the TwoWire transactions during init to check if communication was successful
    if (error == NACK_ADDRESS) {
      return false;
    }

    // initialization was successful
    return true;
  }

  // enable the DS1882 by taking it out of standby (HIGH)
  void DS1882::enable(void) {
    digitalWrite(this->enable_n, LOW);
  }

  // disable the DS1882 via the enable pin
  void DS1882::shutdown(void) {
    digitalWrite(this->enable_n, HIGH);
  }

  // mute the DS1882 by setting maximum attenuation
  bool DS1882::mute(void) {
    // Set Potentiometers volume to zero
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(0x00 + DS1882_MINIMUM_VOL_LEVEL);  // Potentiometer 0
      this->pWire->write(0x40 + DS1882_MINIMUM_VOL_LEVEL);  // Potentiometer 1
    this->pWire->endTransmission();
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();
    
    // use the TwoWire transaction to check if communication was successful
    if (error == NACK_ADDRESS) {
      return false;
    }
    return true;
  }

  // unmute the DS1882 amplifier by restoring previous attenuation
  bool DS1882::unmute(void) {
    // Set Potentiometers volume to current setting
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(0x00 + channel_attenuation[0]);  // Potentiometer 0
      this->pWire->write(0x40 + channel_attenuation[1]);  // Potentiometer 1
    twi_error_type_t error = (twi_error_type_t)this->pWire->endTransmission();
    
    // use the TwoWire transaction to check if communication was successful
    if (error == NACK_ADDRESS) {
      return false;
    }
    return true;
  }

  // set the potentiometers to a value between 0 [min] and 63 [max]
  void DS1882::volume(uint8_t value, channels_t channel) {
    // if the value and channel are within allowable range
    if ((value <= DS1882_MAXIMUM_VOL_LEVEL)) {
      
      switch (channel) {
        case Mono_P0: {
          channel_attenuation[0] = value;

          // Set Potentiometer 0 volume to current setting
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(0x00 + channel_attenuation[0]);  // register address and data
          this->pWire->endTransmission();
        }
        break;

        case Mono_P1: {
          channel_attenuation[1] = value;

          // Set Potentiometer 1 volume to current setting
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(0x40 + channel_attenuation[1]);  // register address and data
          this->pWire->endTransmission();
        }
        break;

        case Stereo: {
          memset(channel_attenuation, value, sizeof(channel_attenuation));

          // Set Potentiometer 0 volume to current setting
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(0x00 + channel_attenuation[0]);  // register address and data
          this->pWire->endTransmission();

          // Set Potentiometer 1 volume to current setting
          this->pWire->beginTransmission(i2c_address);
            this->pWire->write(0x40 + channel_attenuation[1]);  // register address and data
          this->pWire->endTransmission();
        }
        break;
      }
    }
  }
}

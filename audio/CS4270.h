/*
 * CS4270.h - Cirrus CS4270 Stereo CODEC for Arduino
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

#ifndef CIRRUSLOGIC_CS4270_H
#define CIRRUSLOGIC_CS4270_H

  #include <Arduino.h>
  #include <Wire.h>

  // define the DS1882 I2C addresses, by default is hardware configured to 0x28
  #define CS4270_DEFAULT_I2CADDR ((uint8_t)0x48)

  // min/max volume level of theDS1882 potentiometer
  #define CS4270_MINIMUM_ATTENUATION  (0U)
  #define CS4270_MAXIMUM_ATTENUATION  (127U)

  // define the CS4270 configuration register options
  #define CS4270_LOOPBACK (true)
  #define CS4270_DEEMPHAS (false)
  #define CS4270_AUTOMUTE (false)

  // Define CS4270 register addresses
  #define CS4270_DEVICEID 0x01  //device ID, read-only
  #define CS4270_PWR_CTRL 0x02  //power control
  #define CS4270_MDE_CTRL 0x03  //mode control
  #define CS4270_ADDACTRL 0x04  //ADC and DAC control
  #define CS4270_TRN_CTRL 0x05  //transition control
  #define CS4270_MTE_CTRL 0x06  //mute
  #define CS4270_DAC_VOLA 0x07  //DAC Channel A volume
  #define CS4270_DAC_VOLB 0x08  //DAC Channel B volume
  #define CS4270_MAP_INCR 0x80  //I2C MAP auto-increment


  class CS4270 {
    public:
      // These are available channels for setting volume
      enum channels_t {
        Ch_A,
        Ch_B,
        Stereo,
      };
  
      CS4270(uint8_t i2c_addr, uint8_t reset_n, TwoWire *pWire);

      void init(void);
      void enable(void);
      void shutdown(void);
      void mute(void);
      void unmute(void);
      void volume(uint8_t value, channels_t channel);

    private:
      const uint8_t i2c_addr;
      const uint8_t reset_n;
      uint8_t channel_attenuation[2];
      TwoWire *pWire;
  };

#endif

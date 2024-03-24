/*
 * DS1882.h - Maxim DS1882 Dual Log Audio Digital Potentiometer for Arduino
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

#ifndef MAXIM_DS1882_H
#define MAXIM_DS1882_H

  #include <Arduino.h>
  #include <Wire.h>

  // define the DS1882 I2C addresses, by default is hardware configured to 0x28
  #define DS1882_DEFAULT_I2CADDR ((uint8_t)0x28)

  // min/max volume level of theDS1882 potentiometer
  #define DS1882_MINIMUM_VOL_LEVEL  ((uint8_t)0U)
  #define DS1882_MAXIMUM_VOL_LEVEL  ((uint8_t)63U)

  // use volatile or non-volatile memory storage
  #define USE_VOLATILE_MEMORY_STORAGE ((uint8_t)0U)

  // enable ZCD to reduce artifacts when changing volume levels
  #define ENABLE_ZERO_CROSSING_DETECT ((uint8_t)0U)

  // potentiometer configuration option, must be set to 1
  #define POTENTIOMETER_CONFIG_OPTION ((uint8_t)1U)

  class DS1882 {
    public:
      // These are available channels for setting volume
      enum channels_t {
        Ch_0,
        Ch_1,
        Stereo,
      };

      DS1882(uint8_t i2c_addr, uint8_t enable_n, TwoWire *pWire);

      void init(void);
      void enable(void);
      void shutdown(void);
      void mute(void);
      void unmute(void);
      void volume(uint8_t value, channels_t channel);

    private:
      const uint8_t i2c_addr;
      const uint8_t enable_n;
      uint8_t channel_attenuation[2];
      TwoWire *pWire;
  };

#endif

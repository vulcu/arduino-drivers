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

  namespace DS1882 {
    namespace DS1882Types {
      /*! @enum TwoWire error types */
      enum twi_error_type_t {
        NO_ERROR = 0,
        TX_BUFFER_OVERFLOW, 
        NACK_ADDRESS, 
        NACK_DATA, 
        OTHER, 
        TIME_OUT
      };
    }

    class DS1882 {
      public:
        /*! @enum These are available channels for setting volume */
        enum channels_t {
          Mono_P0 = 0,
          Mono_P1,
          Stereo,
        };

        /*! @brief Class constructor
        *
        * @details Create an instance of the class to represent a single DS1882 hardware device
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class
        */
        DS1882(uint8_t i2c_address, uint8_t enable_n, TwoWire *pWire);

        /*! @brief  Initialize the DS1882
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        bool init(void);

        /*! @brief  Enable the DS1882
        *
        * @details Take the device out of hardware reset by setting 'enable_n' pin HIGH
        */
        void enable(void);

        /*! @brief  Shutdown the DS1882
        *
        * @details Put the device into hardware reset by setting 'enable_n' pin LOW
        */
        void shutdown(void);

        /*! @brief  Mute the DS1882
        *
        * @details Set both DS1882 channels to the minimum volume level (0)
        */
        void mute(void);

        /*! @brief  Unmute the DS1882
        *
        * @details Set both DS1882 channels to their most recently set value
        */
        void unmute(void);

        /*! @brief  Set the DS1882 volume (aka potetiometer position)
        *
        * @details Set volume level of the DS1882, from 0-63 and for either or both channels
        * 
        * @param value    The desired volume setting (0-63)
        * @param channel  The channel to apply this setting to (using channels_t)
        */
        void volume(uint8_t value, channels_t channel);

      private:
        const uint8_t i2c_address;
        const uint8_t enable_n;
        uint8_t channel_attenuation[2];
        TwoWire *pWire;
    };
  }

#endif

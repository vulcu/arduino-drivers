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

  // define the CS4270 I2C addresses, by default is hardware configured to 0x48
  #define CS4270_DEFAULT_I2CADDR ((uint8_t)0x48)

  // min/max volume level of theDS1882 potentiometer
  #define CS4270_MINIMUM_ATTENUATION  ((uint8_t)0U)
  #define CS4270_MAXIMUM_ATTENUATION  ((uint8_t)127U)

  // define the CS4270 configuration register options
  #define CS4270_LOOPBACK (true)
  #define CS4270_DEEMPHAS (false)
  #define CS4270_AUTOMUTE (false)

  // Define CS4270 register addresses
  #define CS4270_DEVICEID ((uint8_t)0x01) //device ID, read-only
  #define CS4270_PWR_CTRL ((uint8_t)0x02) //power control
  #define CS4270_MDE_CTRL ((uint8_t)0x03) //mode control
  #define CS4270_ADDACTRL ((uint8_t)0x04) //ADC and DAC control
  #define CS4270_TRN_CTRL ((uint8_t)0x05) //transition control
  #define CS4270_MTE_CTRL ((uint8_t)0x06) //mute
  #define CS4270_DAC_VOLA ((uint8_t)0x07) //DAC Channel A volume
  #define CS4270_DAC_VOLB ((uint8_t)0x08) //DAC Channel B volume
  #define CS4270_MAP_INCR ((uint8_t)0x80) //I2C MAP auto-increment

  namespace CS4270 {
    namespace CS4270Types {
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

    class CS4270 {
      public:
        /*! @enum These are available channels for setting volume */
        enum channels_t {
          Mono_ChA = 0,
          Mono_ChB,
          Stereo,
        };
    
        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class
        */
        CS4270(uint8_t i2c_address, uint8_t reset_n, TwoWire *pWire);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        bool init(void);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void enable(void);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void shutdown(void);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void mute(void);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void unmute(void);

        /*! @brief  Initialize the CS4270
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void volume(uint8_t value, channels_t channel);

      private:
        const uint8_t i2c_address;
        const uint8_t reset_n;
        uint8_t channel_attenuation[2];
        TwoWire *pWire;
    };
  }

#endif

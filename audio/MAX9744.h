/*
 * MAX9744.h - Maxim MAX9744 Class-D Amplifier driver for Arduino
 * Copyright (c) 2017 Winry R. Litwa-Vulcu. All rights reserved.
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

#ifndef MAXIM_MAX9744_H
#define MAXIM_MAX9744_H

  #include <avr/pgmspace.h>
  #include <Wire.h>

  // default I2C address for the MAX9744
  #define MAX9744_DEFAULT_I2CADDR    (0x4B)

  // min/max volume level of the MAX9744 amplifier
  #define MAX9744_MINIMUM_VOL_LEVEL  (  0U)
  #define MAX9744_MAXIMUM_VOL_LEVEL  ( 63U)

  namespace MAX9744{
    namespace MAX9744Types {
      /*! @enum TwoWire error types */
      typedef enum twi_error_type_t {
        NO_ERROR = 0,
        TX_BUFFER_OVERFLOW, 
        NACK_ADDRESS, 
        NACK_DATA, 
        OTHER, 
        TIME_OUT
      };
    }

    class MAX9744 {
      public:
        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class
        */
        MAX9744(uint8_t i2c_address, uint8_t mute_p, uint8_t shutdown_n, TwoWire *pWire);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        bool init(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void enable(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void invertMuteLogic(bool invert_mute);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void mute(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void shutdown(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void unmute(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void volume(uint8_t value);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        inline int16_t getGainAtVolumeIndex(uint8_t index);

      private:
        bool invert_mute;
        const uint8_t i2c_address;
        const uint8_t mute_p;
        const uint8_t shutdown_n;
        static const int16_t MAX9744Gain_milliBels[64] PROGMEM;
        TwoWire *pWire;
    };
  }

#endif

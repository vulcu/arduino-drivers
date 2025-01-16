/*
 * AD5290.h - Analog Devices AD5290 Digital Potentiometer for Arduino
 * Copyright (c) 2025 Winry R. Litwa-Vulcu. All rights reserved.
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

#ifndef ADI_AD5290_H
#define ADI_AD5290_H

  #include <Arduino.h>
  #include <SPI.h>

  // define the AD5290 SPI maximum bus speed, data order, and transmission mode
  #define AD5290_SPI_SPEEDMAXIMUM (4000000U)
  #define AD5290_SPI_DATAORDER    (MSBFIRST)
  #define AD5290_SPI_DATAMODE     (SPI_MODE0)

  // min/max volume level of theDS1882 potentiometer
  #define AD5290_MINIMUM_WIPER_VALUE  (  0U)
  #define AD5290_MIDPOINT_WIPER_VALUE (127U)
  #define AD5290_MAXIMUM_WIPER_VALUE  (255U)

  namespace AD5290 {
    namespace AD5290Types {
      /*! @enum error types */
      enum spi_error_type_t {
        NO_ERROR = 0, 
        NO_RESPONSE
      };
    }

    class AD5290 {
      public:
        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param spi_chip_select The Arduino pin used as SPI Chip Select (CS) for the physical device
        * 
        * TODO: add support for daisy-chained AD5290 configuration
        */
        AD5290(uint8_t spi_chip_select);

        /*! @brief  Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        bool init(void);

        /*! @brief  Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * TODO: this may not work due depending on design of AD5290 registers, needs testing
        */
        uint8_t get(void);

        /*! @brief  Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void set(uint8_t wiper_value);

      private:
        const uint8_t spi_chip_select;
        uint8_t wiper_value;

        /*! @brief  Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        */
        void beginTransaction(void);

        /*! @brief  Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        */
        void endTransaction(void);
    };
  }

#endif

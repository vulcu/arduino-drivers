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

  // min/max volume level of the AD5290 potentiometer
  #define AD5290_MINIMUM_WIPER_VALUE  ((uint8_t)  0U)
  #define AD5290_MIDPOINT_WIPER_VALUE ((uint8_t)127U)
  #define AD5290_MAXIMUM_WIPER_VALUE  ((uint8_t)255U)

  // number of devices SPI daisy-chained together
  #define AD5290_MINIMUM_DEVICE_COUNT ( 1U)
  #define AD5290_MAXIMUM_DEVICE_COUNT (16U)

  namespace AD5290 {
    namespace AD5290Types {
    }

    class AD5290 {
      public:
        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param spi_chip_select The Arduino pin used as SPI Chip Select (CS) for the physical device
        */
        AD5290(uint8_t spi_chip_select, uint32_t spi_bus_speed);
        AD5290(uint8_t spi_chip_select, uint32_t spi_bus_speed, uint8_t number_of_devices);

        /*! @brief Initialize the AD5290
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration
        * 
        * @param   void
        * @returns bool   'True' if register initialization readback was successful
        */
        bool init(void);

        /*! @brief Set the wiper position of a single AD5290
        *
        * @details Write data to the wiper value register via SPI to a single AD5290
        * 
        * @param   value    The value to programmably set the potentiometer value [0-255]
        * @returns void
        */
        void set(uint8_t value);

        /*! @brief Set the wiper position of multiple daisy-chained AD5290
        *
        * @details Write data to the wiper value register via SPI to a multiple AD5290
        * 
        * @param    array       An array of values to programmably set the potentiometer [0-255]
        * @param    array_size  The size of the array as returned by sizeof()
        * @returns  bool        'True' if the operation was successful
        */
        bool set(uint8_t* array, size_t array_size);

        /*! @brief Get the wiper position of a single AD5290
        *
        * @details Read data via SPI from the wiper value register of a single AD5290
        * 
        * @param    void
        * @returns  uint8_t   The register value read back from a single AD5290
        */
        uint8_t get(void);

        /*! @brief Get the wiper position of multiple daisy-chained AD5290
        *
        * @details Read data via SPI from the wiper value register of multiple daisy-chained AD5290
        * 
        * @param    array       An array to store values read from the daisy-chained AD5290
        * @param    array_size  The size of the array as returned by sizeof()
        * @returns  bool        'True' if the operation was successful
        */
        bool get(uint8_t* array, size_t array_size);

      private:
        const uint8_t spi_chip_select;
        const uint32_t spi_bus_speed;
        const uint8_t number_of_devices;
        uint8_t wiper_data[];

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

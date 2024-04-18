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

  // array in PROGMEM for storing dB coefficients
  #define DB_FAST_COEFFICIENT_COUNT  ( 25U)

  // Gain range bounds and analysis step size (in 1/100ths of a dB)
  // these values must be hardcoded to match `coeffecients_dB`
  #define MILLIBEL_BOUND_LOWER ((int16_t)-600)
  #define MILLIBEL_BOUND_UPPER ((int16_t)600)
  #define MILLIBEL_STEP_SIZE   ((int16_t)50)

  namespace MAX9744{
    namespace MAX9744Types {
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
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class
        */
        void enable(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void invertMuteLogic(bool invert_mute);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void mute(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void shutdown(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class
        */
        void unmute(void);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void volume(uint8_t value);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        inline int16_t getGainAtVolumeIndex(uint8_t index);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void convertVolumeToGain(const uint8_t start, 
                                 const uint8_t stop, 
                                 int16_t *values, 
                                 const size_t size);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void mapVolumeToBoundedRange(const uint8_t volume, 
                                     uint8_t *volume_map, 
                                     const size_t volume_map_size);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        uint8_t getVolumeMapIndx(const uint16_t audio_level, 
                                 const uint16_t *dB_levels, 
                                 const size_t dB_levels_size);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        void dBFastRelativeLevel(uint16_t *dB_levels, const uint16_t base_level);

        /*! @brief  Initialize the MAX9744
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @param i2c_address The physical device's I2C address
        * @param reset_n     The microcontroller pin connected to the device RESET_L next
        * @param pWire       A pointer to an instance of the TwoWire class 
        */
        uint16_t decayBuffer32(uint16_t *data_buffer, 
                               const size_t buffer_size, 
                               uint16_t const data_mean, 
                               const uint16_t nominal_zero_signal_level);

      private:
        const uint8_t i2c_address;
        const uint8_t mute_p;
        const uint8_t shutdown_n;
        bool invert_mute;
        
        // index for keeping track of the buffer used by decayBuffer32
        uint8_t buffer_index;

        // index for keeping track of the most recent Volume Map index
        uint8_t vm_index_previous;

        TwoWire *pWire;
    };
  }

#endif

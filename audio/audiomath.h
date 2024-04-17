/*
 * audiomath.h - Math library for Opossum BT Aplifier
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

#ifndef AUDIOMATH_H
#define AUDIOMATH_H

  

  /// TODO: Extricate the MAX9744-specific methods and move to MAX9744 driver class
  class Audiomath {
    public:

      /*! @brief  Error-check the incoming ASCII command string
      *
      * @details Detailed description here.
      * 
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @returns int16_t Total valid character count of incoming buffer
      */
      static uint32_t floatTo523(float float_value);

      /*! @brief  Error-check the incoming ASCII command string
      *
      * @details Detailed description here.
      * 
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @returns int16_t Total valid character count of incoming buffer
      */
      static int16_t  quantizeValueToNearest(int16_t value, const uint8_t quantize_step);

      /*! @brief  Error-check the incoming ASCII command string
      *
      * @details Detailed description here.
      * 
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @returns int16_t Total valid character count of incoming buffer
      */
      int16_t getIntFromCharArray(const char *char_array, uint8_t start, size_t array_size);

      /*! @brief  Error-check the incoming ASCII command string
      *
      * @details Detailed description here.
      * 
      * @param   param    Description of the input parameter
      * @param   param    Description of the input parameter
      * @returns uint16_t Total valid character count of incoming buffer
      */
      uint16_t power_uint8(uint8_t base, uint8_t exponent);
  };

#endif

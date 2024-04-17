/*
 * audiomath.cpp - Math library for Opossum BT Aplifier
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

#include <avr/pgmspace.h>

#include "audiomath.h"

uint32_t Audiomath::floatTo523(float float_value) {  
  // multiply float value by 2^23
  int32_t shifted_value = float_value * ((int32_t)1 << 23);
  
  // convert to positive binary
  int32_t positive_binary_value = shifted_value + ((int32_t)1 << 27);

  // ^ operator is bitwise XOR
  if(float_value >= 0)
    return(positive_binary_value ^ 0x08000000);
  else
    return(positive_binary_value ^ 0xF8000000);
}

int16_t Audiomath::quantizeValueToNearest(int16_t value, const uint8_t quantize_step) {
  int8_t modulus = (value % quantize_step);
  if (modulus > 0) {
    if (modulus <= (quantize_step >> 1)) {
      value = value - (int16_t)modulus;
    }
    else {
      value = value + (int16_t)(quantize_step - modulus);
    }
  }
  else if (modulus < 0) {
    if (-modulus <= (quantize_step >> 1)) {
      value = value - (int16_t)modulus;
    }
    else {
      value = value - (int16_t)(quantize_step + modulus);
    }
  }
  return value;
}

/// TODO: fix handling of negative numbers because it's currently broken
/// TODO: fix handling of numbers greater than 69999
int16_t Audiomath::getIntFromCharArray(const char *char_array, uint8_t start, size_t array_size) {
  bool is_negative = false;
  bool is_fractional = false;
  int16_t numeric_value = 0;

  for (uint8_t k = 0; k < array_size; k++) {
    if (char_array[k] == 46) {
      is_fractional = true;
      array_size = k;
    }
  }

  for (uint8_t k = start; k < array_size; k++) {
    if (char_array[k] == 45) {
      if (k != 0) {
        this->writeErrorMsgToSerialBuffer(this->lastError.set(NumericFormat), this->lastError.message);
        return 0;
      }
      is_negative = true;
      k += 1;
    }
    else if (!((char_array[k] > 47) && (char_array[k] < 58))) {
      this->writeErrorMsgToSerialBuffer(this->lastError.set(NonNumeric), this->lastError.message);
      return 0;
    }
    else {
      // math library uses a lot of program space so don't use pow() here
      uint16_t order_of_mag = power_uint8(10, (array_size - (k + 1)));

      /// TODO: Since always multiplying by power of 10 might not need call to power_uint8()
      numeric_value += ((char_array[k] - 48) * order_of_mag);
    }
  }

  /// TODO: Test if this is actually used when number is negative?
  if (is_negative) {
    numeric_value *= -1;
  }

  if (is_fractional) {
    // Input data value was not an integer
    this->pSerial->println(F("Warning: Only integer data values are accepted"));
    this->pSerial->print(F("Requested value rounded towards zero, new value is "));
    this->pSerial->println(numeric_value);
  }

  return numeric_value;
}

uint16_t Audiomath::power_uint8(const uint8_t base, const uint8_t exponent) {
    if (exponent == 0) {
      return (uint16_t)1;
    }
    else if (exponent == 1) {
      return (uint16_t)base;
    }
    else {
      uint16_t output = base;
      for (uint8_t k = 1; k < exponent; k++) {
        output *= base;
      }
      return output;
    }
  }

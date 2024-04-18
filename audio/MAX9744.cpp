/*
 * MAX9744.cpp - Maxim MAX9744 Class-D Amplifier driver for Arduino
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

#include "MAX9744.h"

namespace MAX9744 {
  using namespace MAX9744Types;

  // Coefficient table for fast dB approximations
  static const uint16_t dB_fast_coefficient[DB_FAST_COEFFICIENT_COUNT] PROGMEM =
  {
    2053, 2175, 2303, 2440, 2584,
    2738, 2900, 3072, 3254, 3446,
    3651, 3867, 4096, 4339, 4596,
    4868, 5157, 5462, 5786, 6129,
    6492, 6876, 7284, 7715, 8173
  };

  // MAX9744 amplifier gain levels (dB), stored as milli-Bells 
  // (1/100 of a dB) to allow PROGMEM storage as an int type
  static const int16_t gain_milliBels[64] PROGMEM = 
  {
    -10950,  -9290,   -9030,   -8680,
    -8430,   -8080,   -7830,   -7470,
    -7220,   -6870,   -6620,   -6270,
    -6020,   -5670,   -5420,   -5060,
    -4810,   -4560,   -4370,   -4210,
    -3960,   -3760,   -3600,   -3340,
    -3150,   -2980,   -2720,   -2520,
    -2350,   -2160,   -1970,   -1750,
    -1640,   -1540,   -1440,   -1310,
    -1200,   -1090,   -990,    -890,
    -710,    -600,    -500,    -340,
    -190,    -50,      50,      120,
    160,     200,     240,     290,
    340,     390,     440,     490,
    540,     590,     650,     700,
    760,     820,     880,     950
  };

  // class constructor for MAX9744 amplifier object
  MAX9744::MAX9744(uint8_t i2c_address, uint8_t mute_p, uint8_t shutdown_n, TwoWire *pWire) :
    i2c_address(i2c_address), 
    mute_p(mute_p), 
    shutdown_n(shutdown_n),
    invert_mute(false), 
    buffer_index(0),
    vm_index_previous(DB_FAST_COEFFICIENT_COUNT >> 1) {
    this->shutdown();
    this->pWire = pWire;
  }

  // initialize the MAX9744 and GPIO signals
  bool MAX9744::init(void) {
    pinMode(mute_p, OUTPUT);
    pinMode(shutdown_n, OUTPUT);

    // mute the MAX9744 then take it out of shutdown
    this->mute();
    this->enable();
      
    // use a TwoWire transaction during init to check if communication is working
    this->pWire->beginTransmission(i2c_address);
    twi_error_type_t error = (twi_error_type_t)(this->pWire->endTransmission());
    if (error == NACK_ADDRESS) {
      return false;
    }
    else {
      return true;
    }
  }

  // enable the MAX9744 by taking it out of shutdown (HIGH)
  void MAX9744::enable(void) {
    digitalWrite(shutdown_n, HIGH);
  }

  // invert the mute signal (needed for use with Adafruit MAX9744 board)
  void MAX9744::invertMuteLogic(bool invert_mute) {
    this->invert_mute = invert_mute;
  }

  // mute the MAX9744 amplifier via the MUTE pin
  void MAX9744::mute(void) {
    // if the mute signal is inverted then set LOW to mute
    if (invert_mute) {
      digitalWrite(mute_p, LOW);
    }
    else {
      digitalWrite(mute_p, HIGH);
    }
  }

  // disable the MAX9744 via the GPIO shutdown signal
  void MAX9744::shutdown(void) {
    digitalWrite(shutdown_n, LOW);
  }

  // unmute the MAX9744 amplifier via the MUTE pin
  void MAX9744::unmute(void) {
    // if the mute signal is inverted then set HIGH to unmute
    if (invert_mute) {
      digitalWrite(mute_p, HIGH);
    }
    else {
      digitalWrite(mute_p, LOW);
    }
  }

  // set the amplifier volume to a value between 0 [min] and 63 [max]
  void MAX9744::volume(uint8_t value) {
    // if the value is less than 64 and two-wire is configured
    if (value < MAX9744_MINIMUM_VOL_LEVEL) {
      value = (uint8_t)MAX9744_MINIMUM_VOL_LEVEL;
    } 
    else if (value > MAX9744_MAXIMUM_VOL_LEVEL) {
      value = (uint8_t)MAX9744_MAXIMUM_VOL_LEVEL;
    }
    this->pWire->beginTransmission(i2c_address);
      this->pWire->write(value);
    this->pWire->endTransmission();
  }

  // return the dB gain values correllating amplifier volume settings
  inline int16_t MAX9744::getGainAtVolumeIndex(uint8_t index) {
    return pgm_read_word(&(gain_milliBels[index]));
  }

  // return the dB gain values correllating amplifier volume settings
  void MAX9744::convertVolumeToGain(const uint8_t start, const uint8_t stop, 
                                      int16_t *values, const size_t values_size) {
    uint8_t index_minimum = ((start <= stop) ? start : stop);
    uint8_t index_maximum = ((start  > stop) ? start : stop);
    if ((index_maximum - index_minimum) > values_size) {
      return;   // the `values[]` array is not large enough to contain the requested range
    }
    else {
      for (uint8_t k = 0; k <= values_size; k++) {
        values[k] = this->getGainAtVolumeIndex(index_minimum + k);
      }
    }
  }

  // return a map of volume thresholds based on gain-to-volume levels at present volume setting
  void MAX9744::mapVolumeToBoundedRange(const uint8_t volume, uint8_t *volume_map, 
                                        const size_t volume_map_size) {
    if (volume_map_size != (size_t)DB_FAST_COEFFICIENT_COUNT) {
      return; // volumeMap array isn't sized right so return without doing anything
    }
    uint8_t a = MAX9744_MINIMUM_VOL_LEVEL;
    uint8_t b = MAX9744_MAXIMUM_VOL_LEVEL;
    int16_t gain_current_volume = this->getGainAtVolumeIndex(volume);

    // reset the volume map index used by getVolumeMapIndx() to the default mid value
    vm_index_previous = (DB_FAST_COEFFICIENT_COUNT >> 1);

    // find the upper and lower bounds for volume values
    if (volume != 0) {
      for(int16_t k = volume - 1; k >= 0; k--) {
        if ((this->getGainAtVolumeIndex(k) - gain_current_volume) < MILLIBEL_BOUND_LOWER) {
          a = k + 1;
          break;
        }
      }
    }
    if (volume != MAX9744_MAXIMUM_VOL_LEVEL) {
      for(uint8_t k = volume + 1; k < MAX9744_MAXIMUM_VOL_LEVEL; k++) {
        if ((this->getGainAtVolumeIndex(k) - gain_current_volume) > MILLIBEL_BOUND_UPPER) {
          b = k - 1;
          break;
        }
      }
    }

    uint8_t v_range = (b - a) + 1;
    uint8_t v[v_range];
    for (int16_t k = b; k >= (int16_t)a; k--) {
      v[b - k] = k;
    }

    int16_t c_normal[v_range];
    //int16_t c_offset[v_range] = {0};
    for (uint8_t k = 0; k < v_range; k++) {
      c_normal[k] = this->getGainAtVolumeIndex(v[k]) - gain_current_volume;
    }

    uint8_t skip_zero_index = 0;
    uint8_t map_index = 0;
    for (uint8_t k = 0; k < volume_map_size; k++) {
      if (c_normal[map_index] == 0) {
        skip_zero_index = 1;
      }
      if ((map_index + skip_zero_index) < v_range) {
        if ((MILLIBEL_BOUND_UPPER - c_normal[map_index + skip_zero_index]) 
            < (k * MILLIBEL_STEP_SIZE)) {
          map_index++;
        }
      }
      volume_map[k] = v[map_index];
    }
  }

  // return a hysterisis-filtered volume map index value that corresponds to the mean audio level
  uint8_t MAX9744::getVolumeMapIndx(const uint16_t audio_level, const uint16_t *dB_levels, 
                                      const size_t dB_levels_size) {
    // check dBLevels array is sized correctly, if not then return default mid-array index
    // bit shift to account for dBLevels being uint16_t and sizeof() returning byte count
    if (dB_levels_size != (size_t)(DB_FAST_COEFFICIENT_COUNT << 1)) {
      return (uint8_t)(DB_FAST_COEFFICIENT_COUNT >> 1);
    }
    for (int8_t k = (DB_FAST_COEFFICIENT_COUNT - 1); k >= 0; k--) {
      if ((dB_levels[k] <= audio_level) | (k < 1)) {
        if (abs(vm_index_previous - k) > 1) {
          vm_index_previous = ((vm_index_previous < k) ? 
                              (vm_index_previous + 1) : 
                              (vm_index_previous - 1));
        }
        else if (((vm_index_previous < 2) & (k < vm_index_previous)) |
                ((vm_index_previous > 22) & (k > vm_index_previous))){
          vm_index_previous = k;
        }
        break;
      }
    }
    return vm_index_previous;
  }

  // update the relative dB level bands using currently defined volume level
  void MAX9744::dBFastRelativeLevel(uint16_t *dB_levels, const uint16_t base_level) {
    for(uint8_t k = 0; k < DB_FAST_COEFFICIENT_COUNT; k++) {
      dB_levels[k] = ((uint32_t)base_level * pgm_read_word(&(dB_fast_coefficient[k]))) >> 12;
    }
  }

  // use a 32-value circular buffer to track audio levels, MUST be 32 elements
  uint16_t MAX9744::decayBuffer32(uint16_t *data_buffer, const size_t buffer_size,
                                  const uint16_t data_mean, 
                                  const uint16_t nominal_zero_signal_level) {
    if ((uint16_t)buffer_size != (uint16_t)32) {
      // this method will only return accurate values for arrays with 32 elements
      // if this is not the case then don't do anything and return zero
      return (uint16_t)0;
    }
    else {
      if (this->buffer_index >= 32) {
        this->buffer_index = 0;
      }
      if (data_mean > data_buffer[this->buffer_index]) {
        // if the new value is greater, use value halfway between old and new
        data_buffer[this->buffer_index] = data_buffer[this->buffer_index] +
          ((data_mean - data_buffer[this->buffer_index]) >> 1);
      }
      else if (data_mean < ((nominal_zero_signal_level * (uint16_t)18) >> 4)) {
        // if the latest level is less a small % over the nominal zero signal,
        // there's probs no significant audio signal so don't update buffer
      }
      else {
        // otherwise, decay the current value by approximately 3%
        data_buffer[this->buffer_index] = (data_buffer[this->buffer_index] * 31L) >> 5;
      }
      this->buffer_index++;

      // calculate total sum of exponential buffer array
      uint32_t sum = 0;
      for (uint16_t k = 0; k < 32; k++) {
        sum = sum + data_buffer[k];
      }

      // divide the sum by 32 and return the array mean
      return (uint16_t)(sum >> 5);
    }
  }
}

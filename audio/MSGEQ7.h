/*
 * MSGEQ7.h - MSI MSGEQ7 Seven Band Graphic Equalizer Driver for Arduino
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

#ifndef MSI_MSGEQ7_H
#define MSI_MSGEQ7_H

  namespace MSGEQ7 {
    class MSGEQ7 {
      public:
        MSGEQ7(uint8_t strobe_p, uint8_t dc_out, uint8_t reset_p, bool use_input_pullup);

        void     init(void);
        uint16_t mean(uint16_t array_values[], const size_t array_size);
        void     read(uint16_t array_values[], const size_t array_size);

      private:
        const uint8_t strobe_p;
        const uint8_t dc_out;
        const uint8_t reset_p;
        const bool    use_input_pullup;
    };
  }

#endif

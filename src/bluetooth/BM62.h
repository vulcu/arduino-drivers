/*
 * BM62.h - Microchip BM62 Audio Module Driver for Arduino
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

#ifndef MICROCHIP_BM62_H
#define MICROCHIP_BM62_H

  // check if an AVR sleep mode is defined for this hardware
  #if (defined(__AVR_ATmega161__ ) || defined(__AVR_ATmega162__)  || \
      defined(__AVR_ATmega8515__) || defined(__AVR_XMEGA__)      || \
      defined(SM2) || defined(SM1) || defined(SM))

    #include <avr/sleep.h>
  #endif

  #define BM62_INIT_RESET_CYCLE_WAIT_TIME_MS (10U)
  #define BM62_BYTE_LENGTH_EQ_PRESET         (3U)

  namespace BM62 {
    class BM62 {
      public:
        // These are the equalizer preset modes available on the BM62
        enum eq_preset_t {
          EQ_Off, 
          EQ_Soft, 
          EQ_Bass, 
          EQ_Treble, 
          EQ_Classical, 
          EQ_Rock, 
          EQ_Jazz, 
          EQ_Pop, 
          EQ_Dance,
          EQ_RnB, 
          EQ_Custom,
        };

        /*! @brief Class constructor
          *
          * @details A more elaborate description of the constructor.
          * 
          * @param prgm_sense_n  Pin number for senseing if BM62 module was booted into flash reprogram mode
          * @param reset_n       Pin number for resetting BM62 module
          * @param ind_a2dp_n    Pin number for reading A2DP profile connection status
          * @param pSerial       A pointer to an instance of the Stream class
          */
        BM62(uint8_t prgm_sense_n, uint8_t reset_n, uint8_t ind_a2dp_n, Stream *pSerial);

        void enable(void);
        void enterPairingMode(void);
        void init(void);
        bool isConnected(void);
        void reset(void);
        void setEqualizerPreset(eq_preset_t preset);
        void play(void);
        void pause(void);
        void playPauseToggle(void);
        void stop(void);
        void previous(void);
        void next(void);

      private:
        // 
        const uint8_t prgm_sense_n;
        const uint8_t reset_n;
        const uint8_t ind_a2dp_n;
        Stream* pSerial;

        uint8_t checksum(const uint8_t command[], const uint8_t command_length);
        void    haltIfProgramMode(void);
        void    writeSerialCommand(const uint8_t instruction[], const size_t bytes_command);
    };
  }

#endif

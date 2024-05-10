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

  #define sizeof_member(type, member) (sizeof( ((type *)0)->member ))

  #define INIT_RESET_CYCLE_WAIT_TIME_MS (10U)
  #define TARGET_LENGTH_EQ_PRESET       (3U)

  namespace BM62 {
    namespace BM62_Types {
      // BM62 UART commands for media playback control
      struct music_control_t {
        const size_t size = 3U;

        // < Music_Control (0x04): AVRCP Commands for Music Control >
        const uint8_t play      [3U] = { 0x04, 0x00, 0x05 };
        const uint8_t pause     [3U] = { 0x04, 0x00, 0x06 };
        const uint8_t toggle    [3U] = { 0x04, 0x00, 0x07 };
        const uint8_t stop      [3U] = { 0x04, 0x00, 0x08 };
        const uint8_t next      [3U] = { 0x04, 0x00, 0x09 };
        const uint8_t previous  [3U] = { 0x04, 0x00, 0x0A };
      };

      // BM62 UART commands for audio equalization control
      struct eq_mode_t {
        const size_t size = 3U;

        // < EQ_Mode_Setting 0x1C: Set EQ Mode of BTM for audio playback >
        const uint8_t off       [3U] = { 0x1C, 0x00, 0xFF };
        const uint8_t soft      [3U] = { 0x1C, 0x01, 0xFF };
        const uint8_t bass      [3U] = { 0x1C, 0x02, 0xFF };
        const uint8_t treble    [3U] = { 0x1C, 0x03, 0xFF };
        const uint8_t classical [3U] = { 0x1C, 0x04, 0xFF };
        const uint8_t rock      [3U] = { 0x1C, 0x05, 0xFF };
        const uint8_t jazz      [3U] = { 0x1C, 0x06, 0xFF };
        const uint8_t pop       [3U] = { 0x1C, 0x07, 0xFF };
        const uint8_t dance     [3U] = { 0x1C, 0x08, 0xFF };
        const uint8_t rnb       [3U] = { 0x1C, 0x09, 0xFF };
        const uint8_t user1     [3U] = { 0x1C, 0x0A, 0xFF };
      };

      // BM62 UART commands for system status and control
      struct system_status_t {
        const size_t size = 3U;

        // < MMI action 0x5D: fast enter pairing mode (from non-off mode) >
        const uint8_t fastEnterPairingMode [3U] = { 0x02, 0x00, 0x5D };
      };

      struct uart_command_t {
        // BM62 UART syncword header for serial commands (probably 0xAA, may be 0x00AA)
        // < EEPROM option (0xAE @ bit 4) adds “0x00” as wakeup byte in front of start byte >
        const uint8_t uartSyncHeader [1U] = { 0xAA };

        const music_control_t avrcp;
        const eq_mode_t eq;
        const system_status_t system;
      };
    }


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
        const BM62_Types::uart_command_t uart;
        Stream* pSerial;

        uint8_t checksum(const uint8_t command[], const uint8_t command_length);
        void    haltIfProgramMode(void);
        void    writeSerialCommand(const uint8_t instruction[], const size_t bytes_command);
    };
  }

#endif

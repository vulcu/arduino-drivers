/*
 * storage.h - EEPROM read/write for turntable motor control
 * Copyright (c) 2021 iZotope, Inc. All rights reserved.
 * 
 * Created: 3/4/2021
 * Author : Winry Litwa-Vulcu
 */

#ifndef STORAGE_H
#define STORAGE_H

  #include <avr/pgmspace.h>
  #include <EEPROM.h>             // enables reading/writing values to EEPROM

    // define EEPROM memory location for checking system options (soft-motion, etc.)
  #define EEPROM_SYSTEM_OPTION_REG  ((int16_t)0x00)
  
  // define EEPROM memory location for checking if axes limits are user-defined
  #define EEPROM_MEMORY_STATE_REG   ((int16_t)0x01)
  
  // define EEPROM memory location for checking if axes speeds are user-defined
  #define EEPROM_MEMORY_POINTER_REG ((int16_t)0x02)
  
  // define example EEPROM memory locations for 16-bit values
  #define EEPROM_A0_LOWBYTE  ((int16_t)0x10)
  #define EEPROM_A0_HIGHBYTE ((int16_t)0x11)
  #define EEPROM_A1_LOWBYTE  ((int16_t)0x12)
  #define EEPROM_A1_HIGHBYTE ((int16_t)0x13)
  #define EEPROM_A2_LOWBYTE  ((int16_t)0x14)
  #define EEPROM_A2_HIGHBYTE ((int16_t)0x15)
  #define EEPROM_A3_LOWBYTE  ((int16_t)0x16)
  #define EEPROM_A3_HIGHBYTE ((int16_t)0x17)
  
  // define example EEPROM memory locations for 8-bit values
  #define EEPROM_B0_LOWBYTE  ((int16_t)0x20)
  #define EEPROM_B1_LOWBYTE  ((int16_t)0x21)
  #define EEPROM_B2_LOWBYTE  ((int16_t)0x22)
  #define EEPROM_B3_LOWBYTE  ((int16_t)0x23)

  // initialize these outside of the class otherwise the PROGMEM directive is ignored 
  static const int16_t eeprom_addresses[11] PROGMEM = 
  {
    EEPROM_SYSTEM_OPTION_REG,
    EEPROM_MEMORY_STATE_REG,
    EEPROM_MEMORY_POINTER_REG,
    EEPROM_A0_LOWBYTE,
    EEPROM_A1_LOWBYTE,
    EEPROM_A2_LOWBYTE,
    EEPROM_A3_LOWBYTE,
    EEPROM_B0_LOWBYTE,
    EEPROM_B1_LOWBYTE,
    EEPROM_B2_LOWBYTE,
    EEPROM_B3_LOWBYTE,
  };

  class EEPROM_Storage {
    public:
     enum eeprom_indexes {
        system_option_register, 
        memory_overwrite_register, 
        memory_pointer_register, 
        BankA_0, 
        BankA_1, 
        BankA_2, 
        BankA_3, 
        BankB_0, 
        BankB_1, 
        BankB_2, 
        BankB_3,
      };
    
  private:
    eeprom_indexes storage_index;

  public:
    EEPROM_Storage(void) {
        // class constructor
      }

      // return the storage_index as an enum of type eeprom_indexes
      eeprom_indexes inline getStorageIndex() {
        return storage_index;
      }

      // read EEPROM data at `index` and copy this to `data`
      void read(eeprom_indexes index, int16_t *data) {
        const int16_t eeprom_address = getStartAddressFromIndex(index);
        const uint8_t size_bytes = getByteSizeFromIndex(index);
        const uint8_t size_array = getArraySizeFromIndex(index);
        int16_t eeprom_data[size_array];
        for(uint8_t k = 0; k < size_array; k++) {
          EEPROM.get((eeprom_address + (int16_t)(k * size_bytes)), eeprom_data[k]);
        }
        memcpy(data, eeprom_data, (size_bytes * size_array));
      }

      // reset modified limit or origin EEPROM data to the default value
      void reset(eeprom_indexes index) {
        const int16_t eeprom_address = getStartAddressFromIndex(index);
        const uint8_t size_bytes = getByteSizeFromIndex(index);
        const uint8_t size_array = getArraySizeFromIndex(index);
        int16_t data[size_array] = {0};       // initialize all elements to 0
        switch (index) {
          case BankA_0: {
            // this is a little clunky because we have to pull from program memory,
            // but speed is not a consideration and it saves us 64 bytes of SRAM
            for(uint8_t k = 0; k < size_array; k++) {
              data[k] = (int16_t)pgm_read_word(&(eeprom_addresses[k]));
            }
          }
          break;
          case BankB_0: {
            // this is a little clunky because we have to pull from program memory,
            // but speed is not a consideration and it saves us 64 bytes of SRAM
            for(uint8_t k = 0; k < size_array; k++) {
              data[k] = (int16_t)pgm_read_word(&(eeprom_addresses[k]));
            }
          }
          break;
          default: {
            return; //invalid reset request so don't do anything
          }
          break;
        }
        for(uint8_t k = 0; k < size_array; k++) {
          // EEPROM.put() is polymorphic and will write correct type to EEPROM
          EEPROM.put((eeprom_address + (int16_t)(k * size_bytes)), data[k]);
        }
      }

      // read  from `data` and copy this to EEPROM at `index` address
      void write(eeprom_indexes index, int16_t *data) {
        const int16_t eeprom_address = getStartAddressFromIndex(index);
        const uint8_t size_bytes = getByteSizeFromIndex(index);
        const uint8_t size_array = getArraySizeFromIndex(index);
        for(uint8_t k = 0; k < size_array; k++) {
          // EEPROM.put() is polymorphic and will write correct type to EEPROM
          EEPROM.put((eeprom_address + (int16_t)(k * size_bytes)), data[k]);
        }
      }

    private:
      // retrieve the EEPROM address from program memory based on the Storage Index
      int16_t inline getStartAddressFromIndex(int16_t index) {
        return (int16_t)pgm_read_word(&(eeprom_addresses[index]));
      }

      // calculate size in bytes of each array element based on the Storage Index
      uint8_t inline getByteSizeFromIndex(int16_t index) {
        return (uint8_t)((index) < 0 ? 0 : ((index) < 3 ? 1 : ((index) < 7 ? 2 : 0)));
      }

      // calculate the Array Size based on the Storage Index
      uint8_t inline getArraySizeFromIndex(int16_t index) {
        return (uint8_t)((index) < 0 ? 0 : ((index) < 3 ? 1 : ((index) < 4 ? 4 : ((index) < 7 ? 2 : 0))));
      }
  };

#endif
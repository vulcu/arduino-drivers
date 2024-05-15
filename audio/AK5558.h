/*
 * AK5558.h - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

// consider replacing with #pragma once
#ifndef AK5558_H
#define AK5558_H

  // Power-on delay of Internal PDN release. See note 2b) on p.55 of AK5558 datasheet.
  #define AK5558_INT_PDN_OSCCLK_DELAY_MICROS (850U)

  // define the AK5558 I2C address, by default is hardware configured to 0x10
  #define AK5558_DEFAULT_I2CADDR (0x10)

  // cast register-specific bitmasks as generic type
  #ifndef register_bm
    #define register_bm(bitmask) static_cast<register_bitmask_t>(bitmask)
  #endif

  namespace AK5558 {
    namespace AK5558Types {
      /*! @typedef Register bitmask for accessing specific control bits */
      typedef uint8_t register_bitmask_t;

      /*! @enum Power Management Register */
      enum pwrmgmt1_bitmask_t {
        PW1_BM = (0x01 << 0),  
        PW2_BM = (0x01 << 1), 
        PW3_BM = (0x01 << 2), 
        PW4_BM = (0x01 << 3), 
        PW5_BM = (0x01 << 4), 
        PW6_BM = (0x01 << 5), 
        PW7_BM = (0x01 << 6),  
        PW8_BM = (0x01 << 7),  
      };

      /*! @enum Channel Summing and Timing Reset Register */
      enum pwrmgmt2_bitmask_t {
        RSTN_BM  = (0x01 << 0), 
        MONO1_BM = (0x01 << 1), 
        MONO2_BM = (0x01 << 2), 
      };

      /*! @enum Clocking, DAI Mode, HP Filter Register */
      enum control1_bitmask_t {
        HFPE_BM = (0x01 << 0), 
        DIF0_BM = (0x01 << 1), 
        DIF1_BM = (0x01 << 2), 
        CKS0_BM = (0x01 << 3), 
        CKS1_BM = (0x01 << 4), 
        CKS2_BM = (0x01 << 5), 
        CKS3_BM = (0x01 << 6), 
      };

      /*! @enum TDM Mode Selection Register */
      enum control2_bitmask_t {
        TDM0_BM = (0x01 << 5), 
        TDM1_BM = (0x01 << 6), 
      };

      /*! @enum LP Filter, DSD Mode Register */
      enum control3_bitmask_t {
        SLOW_BM = (0x01 << 0), 
        SD_BM   = (0x01 << 1), 
      };

      /*! @enum DSD Configuration Register */
      enum dsd_bitmask_t {
        DSDSEL0_BM = (0x01 << 0), 
        DSDSEL1_BM = (0x01 << 1), 
        DCKB_BM    = (0x01 << 2), 
        PMOD_BM    = (0x01 << 3), 
        DCKS_BM    = (0x01 << 5), 
      };

      /*! @enum AK5558 register address index */
      enum register_pointer_t {
        PWRMGMT1 = 0x00,  //Power Management
        PWRMGMT2 = 0x01,  //Channel Summing and Timing Reset
        CONTROL1 = 0x02,  //Clocking, DAI Mode, HP Filter
        CONTROL2 = 0x03,  //TDM Mode Selection
        CONTROL3 = 0x04,  //LP Filter, DSD Mode
        DSD      = 0x05,  //DSD Configuration
        TEST1    = 0x06,  //Test Register (must be 0x00)
        TEST2    = 0x07,  //Test Register (must be 0x00)
      };

      /*! @enum AK5558 channel selection */
      enum channel_select_t {
        All = 0,
        Ch1 = 1,  
        Ch2 = 2, 
        Ch3 = 3, 
        Ch4 = 4, 
        Ch5 = 5, 
        Ch6 = 6, 
        Ch7 = 7,  
        Ch8 = 8, 
      };

      /*! @enum TwoWire error types */
      enum twi_error_type_t {
        NO_ERROR = 0,
        TX_BUFFER_OVERFLOW, 
        NACK_ADDRESS, 
        NACK_DATA, 
        OTHER, 
        TIME_OUT,
      };
    }

    /*! @brief AKM AK5558 Analog-to-Digital Converter driver for Arduino
    *
    * @details A more elaborate description of the class.
    */
    class AK5558 {
      public:
        AK5558Types::channel_select_t channel;

        /*! @brief Class constructor
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param i2c_address The physical device's I2C address
         * @param reset_n     The microcontroller pin connected to the device RESET_L next
         * @param pWire       A pointer to an instance of the TwoWire class
         */
        AK5558(const uint8_t i2c_address, const uint8_t reset_n, TwoWire *pWire);

        /*! @brief  Initialize the AK5558
         *
         * @details Initialize the device and write default config values to all registers
         * 
         * @warning This will reset all device registers to the default configuration 
         */
        bool init(void);

        /*! @brief  Enable the AK5558 by taking it out of reset
         *
         * @details A more elaborate description of the class member.
         */
        void enable(void);

        /*! @brief  Shutdown AK5558 using the internal power control
         *
         * @details A more elaborate description of the class member.
         */
        void shutdown(void);

        /*! @brief  Reset the AK5558 using the reset logic signal
         *
         * @details A more elaborate description of the class member.
         */
        void reset(void);

        /*! @brief  Unmute the AK5558
         *
         * @details A more elaborate description of the class member.
         */
        void mute(AK5558Types::channel_select_t channel);
        
        /*! @brief  Unmute the AK5558
         *
         * @details A more elaborate description of the class member.
         */
        void unmute(AK5558Types::channel_select_t channel);

      private:
        const uint8_t i2c_address;
        const uint8_t reset_n;
        uint8_t active_config[8];
        static const uint8_t default_config[8] PROGMEM;
        TwoWire *pWire;

        /*! @brief Get the value of an 8-bit register
          *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The input port register bit to read
         * 
         * @returns 
         */
        uint8_t getRegister(AK5558Types::register_pointer_t register_pointer);

        /*! @brief Get value of a specific register bit
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The input port register to read
         * @param register_bitmask_t The register bit index to read
         * 
         * @returns 
         */
        bool getRegisterBit(AK5558Types::register_pointer_t register_pointer, 
                            AK5558Types::register_bitmask_t bitmask);

        /*! @brief Set the value of an 8-bit register
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The output port register to write
         * @param uint8_t            The boolean value to write (0 or 1)
         */
        void setRegister(AK5558Types::register_pointer_t register_pointer, uint8_t value);

        /*! @brief Set value of a specific register bit
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The output port register to write
         * @param register_bitmask_t The register bit index to write
         * @param bool               The boolean value to write (0 or 1)
         */
        void setRegisterBit(AK5558Types::register_pointer_t register_pointer, 
                            AK5558Types::register_bitmask_t bitmask, 
                            bool value);

        /*! @brief Reset an AK5558 register to the value specified by the default config
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The output port register to write
         */
        void writeDefaultConfigToRegister(AK5558Types::register_pointer_t register_pointer);

        /*! @brief Reset the AK5558 configuration in memory
         *
         * @details Reset the device configuration in SRAM to the default configuration.
         * 
         * @warning This will reset the device configuration in memory. It will
         *          not apply these changes to the physical device registers.
         */
        void resetActiveConfig(void);
    };
  }

#endif
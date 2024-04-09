/*
 * AK5558.h - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

// consider replacing with #pragma once
#ifndef AK5558_H
#define AK5558_H

  // Power-on delay of Internal PDN release. See note 2b) on p.55 of AK5558 datasheet.
  #define AK5558_INT_PDN_OSCCLK_DELAY_MS (10U)

  // define the AK5558 I2C address, by default is hardware configured to 0x11
  #define AK5558_DEFAULT_I2CADDR (0x11)

  namespace AK5558 {
    namespace AK5558Types {
      /*! @enum Power Management Register */
      enum pwrmgmt1_t {
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
      enum pwrmgmt2_t {
        RSTN_BM  = (0x01 << 0), 
        MONO1_BM = (0x01 << 1), 
        MONO2_BM = (0x01 << 2), 
      };

      /*! @enum Clocking, DAI Mode, HP Filter Register */
      enum control1_t {
        HFPE_BM = (0x01 << 0), 
        DIF0_BM = (0x01 << 1), 
        DIF1_BM = (0x01 << 2), 
        CKS0_BM = (0x01 << 3), 
        CKS1_BM = (0x01 << 4), 
        CKS2_BM = (0x01 << 5), 
        CKS3_BM = (0x01 << 6), 
      };

      /*! @enum TDM Mode Selection Register */
      enum control2_t {
        TDM0_BM = (0x01 << 5), 
        TDM1_BM = (0x01 << 6), 
      };

      /*! @enum LP Filter, DSD Mode Register */
      enum control3_t {
        SLOW_BM = (0x01 << 0), 
        SD_BM   = (0x01 << 1), 
      };

      /*! @enum DSD Configuration Register */
      enum dsd_t {
        DSDSEL0_BM = (0x01 << 0), 
        DSDSEL1_BM = (0x01 << 1), 
        DCKB_BM    = (0x01 << 2), 
        PMOD_BM    = (0x01 << 3), 
        DCKS_BM    = (0x01 << 5), 
      };

      /*! @enum AK5558 register address index */
      enum AK5558_register_index {
        PWRMGMT1 = 0,  //Power Management
        PWRMGMT2,      //Channel Summing and Timing Reset
        CONTROL1,      //Clocking, DAI Mode, HP Filter
        CONTROL2,      //TDM Mode Selection
        CONTROL3,      //LP Filter, DSD Mode
        DSD,           //DSD Configuration
        TEST1,         //Test Register (must be 0x00)
        TEST2,         //Test Register (must be 0x00)
      };

      /*! @enum TwoWire error types */
      typedef enum twi_error_type_t {
        NO_ERROR = 0,
        TX_BUFFER_OVERFLOW, 
        NACK_ADDRESS, 
        NACK_DATA, 
        OTHER, 
        TIME_OUT
      };
    }

    /*! @brief AKM AK5558 Analog-to-Digital Converter driver for Arduino
    *
    * @details A more elaborate description of the class.
    */
    class AK5558 {
      public:
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

        /*! @brief  Disable the AK5558 by placing it in reset
        *
        * @details A more elaborate description of the class member.
        */
        void shutdown(void);

        /*! @brief  Invert mute signal logic
        *
        * @details A more elaborate description of the class member.
        */
        void invertMuteLogic(const bool invert_mute);

        /*! @brief  Unmute the AK5558
        *
        * @details A more elaborate description of the class member.
        */
        void mute(void);
        
        /*! @brief  Unmute the AK5558
        *
        * @details A more elaborate description of the class member.
        */
        void unmute(void);

      private:
        bool invert_mute;
        const uint8_t i2c_address;
        const uint8_t reset_n;
        uint8_t active_config[8];
        static const uint8_t default_config[8] PROGMEM;
        TwoWire *pWire;

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
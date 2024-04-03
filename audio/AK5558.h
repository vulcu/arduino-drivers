/*
 * AK5558.h - AKM AK5558 Analog-to-Digital Converter driver for Arduino
 */

// consider replacing with #pragma once
#ifndef AK5558_H
#define AK5558_H

  namespace AKM {
    namespace AK5558Types {
      // #define PW1_BM     (0x01 << 0)
      // #define PW2_BM     (0x01 << 1)
      // #define PW3_BM     (0x01 << 2)
      // #define PW4_BM     (0x01 << 3)
      // #define PW5_BM     (0x01 << 4)
      // #define PW6_BM     (0x01 << 5)
      // #define PW7_BM     (0x01 << 6)
      // #define PW8_BM     (0x01 << 7)

      // #define RSTN_BM    (0x01 << 0)
      // #define MONO1_BM   (0x01 << 1)
      // #define MONO2_BM   (0x01 << 2)

      // #define HFPE_BM    (0x01 << 0)
      // #define DIF0_BM    (0x01 << 1)
      // #define DIF1_BM    (0x01 << 2)
      // #define CKS0_BM    (0x01 << 3)
      // #define CKS1_BM    (0x01 << 4)
      // #define CKS2_BM    (0x01 << 5)
      // #define CKS3_BM    (0x01 << 6)

      // #define TDM0_BM    (0x01 << 5)
      // #define TDM1_BM    (0x01 << 6)

      // #define SLOW_BM    (0x01 << 0)
      // #define SD_BM      (0x01 << 1)

      // #define DSDSEL0_BM (0x01 << 0)
      // #define DSDSEL1_BM (0x01 << 1)
      // #define DCKB_BM    (0x01 << 2)
      // #define PMOD_BM    (0x01 << 3)
      // #define DCKS_BM    (0x01 << 5)
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
        * @param i2c_addr The physical device's I2C address
        * @param reset_n  The microcontroller pin connected to the device RESET_L next
        * @param Wire     A pointer to an instance of the TwoWire class
        */
        AK5558(const uint8_t i2c_addr, const uint8_t reset_n, TwoWire *Wire);

        /*! @brief  Initialize the AK5558
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void init(void);

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
        /*! @enum AK5558 register address index */
        enum AK5558_register_index {
          AK5558_PWRMGMT1 = 0,  //Power Management
          AK5558_PWRMGMT2,      //Channel Summing and Timing Reset
          AK5558_CONTROL1,      //Clocking, DAI Mode, HP Filter
          AK5558_CONTROL2,      //TDM Mode Selection
          AK5558_CONTROL3,      //LP Filter, DSD Mode
          AK5558_DSD,           //DSD Configuration
          AK5558_TEST1,         //Test Register (must be 0x00)
          AK5558_TEST2,         //Test Register (must be 0x00)
        };

        const uint8_t i2c_address;
        const uint8_t reset_n;
        bool invert_mute;
        uint8_t active_config[8];
        TwoWire *Wire;

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
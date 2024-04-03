/*
 * PCAL6408A.h - NXP PCAL6408A IO Expander driver for Arduino
 */

// consider replacing with #pragma once
#ifndef PCAL6408A_H
#define PCAL6408A_H

  namespace NXP {
    namespace PCAL6408ATypes {
      /*! @enum PCAL6408A register bitmasks */
      enum register_bitmask_t {
        BIT0 = (0x01 << 0),  
        BIT1 = (0x01 << 1), 
        BIT2 = (0x01 << 2), 
        BIT3 = (0x01 << 3), 
        BIT4 = (0x01 << 4), 
        BIT5 = (0x01 << 5), 
        BIT6 = (0x01 << 6),  
        BIT7 = (0x01 << 7),  
      };

      /*! @enum PCAL6408A current control register bitmasks */
      enum current_control_bitmask_t {
        CC0 = (0x01 << 0),  
        CC1 = (0x01 << 2), 
        CC2 = (0x01 << 4), 
        CC3 = (0x01 << 6),  
      };

      /*! @enum PCAL6408A pointer register command bytes */
      enum register_pointer_t {
        INPUT_PORT_PTR          = 0x00, // Input Port Register (Read-Only)
        OUTPUT_PORT_PTR         = 0x01, // Output Port Register
        POLARITY_INVERSION_PTR  = 0x02, // Polarity Inversion (Input Only)
        CONFIGURATION_PTR       = 0x03, // Pin Direction (I/O) Configuration
        DRIVE_STRENGTH_0_PTR    = 0x40, // Output Drive Strength (Port 0-3)
        DRIVE_STRENGTH_1_PTR    = 0x41, // Output Drive Strength (Port 4-7)
        INPUT_LATCH_PTR         = 0x42, // Enable/Disable Input Pin Latching
        PULLUP_PULLDOWN_EN_PTR  = 0x43, // Enable/Disable Pin PU/PD Resistor
        PULLUP_PULLDOWN_SEL_PTR = 0x44, // Configure Pin PU/PD Resistor Mode
        INTERRUPT_MASK_PTR      = 0x45, // Enable/Disable Interrupts Per-Pin
        INTERRUPT_STATUS_PTR    = 0x46, // Interrupt Status (Read-Only)
        OUTPUT_PORT_CONFIG_PTR  = 0x4F, // Set Outputs as Push/Pull or OD
      };

      /*! @enum PCAL6408A register name index */
      enum register_name_t {
        INPUT_PORT          = 0,  // Input Port Register (Read-Only)
        OUTPUT_PORT,              // Output Port Register
        POLARITY_INVERSION,       // Polarity Inversion (Input Only)
        CONFIGURATION,            // Pin Direction (I/O) Configuration
        DRIVE_STRENGTH_0,         // Output Drive Strength (Port 0-3)
        DRIVE_STRENGTH_1,         // Output Drive Strength (Port 4-7)
        INPUT_LATCH,              // Enable/Disable Input Pin Latching
        PULLUP_PULLDOWN_EN,       // Enable/Disable Pin PU/PD Resistor
        PULLUP_PULLDOWN_SEL,      // Configure Pin PU/PD Resistor Mode
        INTERRUPT_MASK,           // Enable/Disable Interrupts Per-Pin
        INTERRUPT_STATUS,         // Interrupt Status (Read-Only)
        OUTPUT_PORT_CONFIG,       // Set Outputs as Push/Pull or OD
      };
    }

    class PCAL6408A {
      public:
        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param i2c_addr The physical device's I2C address
        * @param reset_n  The microcontroller pin connected to the device RESET_L next
        * @param pWire     A pointer to an instance of the TwoWire class
        */
        PCAL6408A(const uint8_t i2c_addr, 
                  const uint8_t reset_n, 
                  const uint8_t interrupt_n, 
                  TwoWire* pWire);

        /*! @brief  Initialize the PCAL6408A
        *
        * @details Initialize the device and write default config values to all registers
        * 
        * @warning This will reset all device registers to the default configuration 
        */
        void init(void);

        /*! @brief  Enable the PCAL6408A by taking it out of reset
        *
        * @details A more elaborate description of the class member.
        */
        void enable(void);

        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param input_port The input port register bit to read
        * 
        * @returns 
        */
        bool get(PCAL6408ATypes::register_bitmask_t input_port);

        /*! @brief Class constructor
        *
        * @details A more elaborate description of the constructor.
        * 
        * @param output_port The output port register bit to write
        * @param value       The boolean value to write (0 or 1)
        */
        void set(PCAL6408ATypes::register_bitmask_t output_port, bool value);

        /*! @brief  Disable the PCAL6408A by placing it in reset
        *
        * @details A more elaborate description of the class member.
        */
        void shutdown(void);

      private:
        const uint8_t i2c_address;
        const uint8_t interrupt_n;
        const uint8_t reset_n;
        uint8_t active_config[12];
        static const uint8_t default_config[12] PROGMEM;
        TwoWire *pWire;

        /*! @brief Reset the PCAL6408A configuration in memory
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
/*
 * PCAL6408A.h - NXP PCAL6408A IO Expander driver for Arduino
 */

// consider replacing with #pragma once
#ifndef PCAL6408A_H
#define PCAL6408A_H

  enum PCAL6408A_port_index {
      PCAL6408A_PORT0 = 0,  // PCAS6408A Port 0
      PCAL6408A_PORT1,      // PCAS6408A Port 1
      PCAL6408A_PORT2,      // PCAS6408A Port 2
      PCAL6408A_PORT3,      // PCAS6408A Port 3
      PCAL6408A_PORT4,      // PCAS6408A Port 4
      PCAL6408A_PORT5,      // PCAS6408A Port 5
      PCAL6408A_PORT6,      // PCAS6408A Port 6
      PCAL6408A_PORT7,      // PCAS6408A Port 7
    };

  class PCAL6408A {
    public:
      /*! @brief Class constructor
       *
       * @details A more elaborate description of the constructor.
       * 
       * @param i2c_addr The physical device's I2C address
       * @param reset_n  The microcontroller pin connected to the device RESET_L next
       * @param Wire     A pointer to an instance of the TwoWire class
       */
      PCAL6408A(const uint8_t i2c_addr, 
                const uint8_t reset_n, 
                const uint8_t interrupt_n, 
                TwoWire* wire);

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

      /*! @brief  Disable the PCAL6408A by placing it in reset
       *
       * @details A more elaborate description of the class member.
       */
      void shutdown(void);

    private:
      /*! @enum PCAL6408A register address index */
      enum PCAL6408A_register_index {
        PCAL6408A_INPUT_PORT = 0,       // Input Port Register (Read-Only)
        PCAL6408A_OUTPUT_PORT,          // Output Port Register
        PCAL6408A_POLARITY_INVERSION,   // Polarity Inversion (Input Only)
        PCAL6408A_CONFIGURATION,        // Pin Direction (I/O) Configuration
        PCAL6408A_DRIVE_STRENGTH_0,     // Output Drive Strength (Port 0-3)
        PCAL6408A_DRIVE_STRENGTH_1,     // Output Drive Strength (Port 4-7)
        PCAL6408A_INPUT_LATCH,          // Enable/Disable Input Pin Latching
        PCAL6408A_PULLUP_PULLDOWN_EN,   // Enable/Disable Pin PU/PD Resistor
        PCAL6408A_PULLUP_PULLDOWN_SEL,  // Configure Pin PU/PD Resistor Mode
        PCAL6408A_INTERRUPT_MASK,       // Enable/Disable Interrupts Per-Pin
        PCAL6408A_INTERRUPT_STATUS,     // Interrupt Status (Read-Only)
        PCAL6408A_OUTPUT_PORT_CONFIG,   // Set Outputs as Push/Pull or OD
      };

      const uint8_t i2c_address;
      const uint8_t interrupt_n;
      const uint8_t reset_n;
      uint8_t active_config[12];
      TwoWire *Wire;

      /*! @brief Reset the PCAL6408A configuration in memory
       *
       * @details Reset the device configuration in SRAM to the default configuration.
       * 
       * @warning This will reset the device configuration in memory. It will
       *          not apply these changes to the physical device registers.
       */
      void resetActiveConfig(void);
  };

#endif
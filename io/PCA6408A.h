/*
 * PCA6408A.h - PCA6408A IO Expander driver for Arduino
 */

// consider replacing with #pragma once
#ifndef PCA6408A_H
#define PCA6408A_H

  // define the PCA6408A I2C address, by default is hardware configured to 0x20
  #define PCA6408A_DEFAULT_I2CADDR (0x20)

  namespace PCA6408A {
    namespace PCA6408ATypes {
      /*! @enum PCAL6408A current control register bitmasks */
      enum current_control_bitmask_t {
        CC0 = (0x01 << 0),  
        CC1 = (0x01 << 2), 
        CC2 = (0x01 << 4), 
        CC3 = (0x01 << 6),  
      };

      /// TODO: split this up to separate the PCAL6408A Agile I/O registers
      /*! @enum PCA6408A and PCAL6408A pointer register command bytes */
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

      /// TODO: split this up to separate the PCAL6408A Agile I/O registers
      /*! @enum PCA6408A and PCAL6408A register names */
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

    class PCA6408A {
      public:
        /*! @enum PCA6408A register bitmasks */
        enum register_bitmask_t {
          BIT0 = (0x01 << 0),  
          BIT1 = (0x01 << 1), 
          BIT2 = (0x01 << 2), 
          BIT3 = (0x01 << 3), 
          BIT4 = (0x01 << 4), 
          BIT5 = (0x01 << 5), 
          BIT6 = (0x01 << 6),  
          BIT7 = (0x01 << 7),  
        } registers;

        /*! @enum PCA6408A input polarity */
        enum polarity_inversion_t {
          NORMAL = 0,  
          INVERTED,  
        } polarity;

        /*! @brief Class constructor
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param i2c_address The physical device's I2C address
         * @param reset_n     The microcontroller pin connected to the device RESET_L next
         * @param pWire       A pointer to an instance of the TwoWire class
         */
        PCA6408A(const uint8_t i2c_address, 
                  const uint8_t reset_n, 
                  const uint8_t interrupt_n, 
                  TwoWire* pWire);

        /*! @brief  Initialize the PCA6408A
         *
         * @details Initialize the device and write default config values to all registers
         * 
         * @warning This will reset all device registers to the default configuration 
         */
        bool init(void);

        /*! @brief  Enable the PCA6408A by taking it out of reset
         *
         * @details A more elaborate description of the class member.
         */
        void enable(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void shutdown(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        bool isAgileIoAvailable(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        uint8_t readInput(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        bool readInputPin(register_bitmask_t port_pin);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void writeOutput(uint8_t value);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void writeOutputPin(register_bitmask_t port_pin, bool value);
        
        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setPortPinAsInput(register_bitmask_t port_pin);
        
        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setAllPinsAsInput(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setPortPinAsOutput(register_bitmask_t port_pin);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setAllPinsAsOutput(void);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setPortPinPolarity(register_bitmask_t port_pin, polarity_inversion_t polarity);

        /*! @brief  Disable the PCA6408A by placing it in reset
         *
         * @details A more elaborate description of the class member.
         */
        void setAllPinsPolarity(polarity_inversion_t polarity);

      private:
        const uint8_t i2c_address;
        const uint8_t reset_n;
        const uint8_t interrupt_n;
        bool agile_io_available;
        uint8_t active_config[12];
        static const uint8_t default_config[12] PROGMEM;
        TwoWire *pWire;

        /*! @brief Get the value of an 8-bit register
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The input port register bit to read
         * 
         * @returns 
         */
        uint8_t getRegister(PCA6408ATypes::register_pointer_t register_pointer);

        /*! @brief Get value of a specific register bit
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The input port register to read
         * @param register_bitmask_t The register bit index to read
         */
        bool getRegisterBit(PCA6408ATypes::register_pointer_t register_pointer, 
                            register_bitmask_t bitmask);

        /*! @brief Set the value of an 8-bit register
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The output port register to write
         * @param uint8_t            The boolean value to write (0 or 1)
         */
        void setRegister(PCA6408ATypes::register_pointer_t register_pointer, uint8_t value);

        /*! @brief Set value of a specific register bit
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_pointer_t The output port register to write
         * @param register_bitmask_t The register bit index to write
         * @param bool               The boolean value to write (0 or 1)
         */
        void setRegisterBit(PCA6408ATypes::register_pointer_t register_pointer, 
                            register_bitmask_t bitmask, 
                            bool value);

        /*! @brief Reset an AK5558 register to the value specified by the default config
         *
         * @details A more elaborate description of the constructor.
         * 
         * @param register_name_t    The name of the output port register to write
         * @param register_pointer_t The output port register to write
         */
        void writeDefaultConfigToRegister(PCA6408ATypes::register_name_t register_name,
                                          PCA6408ATypes::register_pointer_t register_pointer);

        /*! @brief Reset the PCA6408A configuration in memory
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
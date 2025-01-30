/*
 * AD5290.h - Analog Devices AD5290 Digital Potentiometer for Arduino
 * Copyright (c) 2025 Winry R. Litwa-Vulcu. All rights reserved.
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

#include "AD5290.h"

namespace AD5290 {
  using namespace AD5290Types;

  // class constructor for AD5290 object
  AD5290::AD5290(uint8_t spi_chip_select) : 
    spi_chip_select(spi_chip_select), 
    wiper_value(0U) {
      SPI.begin();
  }

  // initialize and configure the device
  bool AD5290::init(void) {
    // configure SPI interface and begin the SPI transaction
    this->beginTransaction();

    // send readback value to ensure SPI is working, then set potentiometer value to midpoint value
    SPI.transfer(0xAA);
    uint16_t spi_received_value = SPI.transfer(AD5290_MIDPOINT_WIPER_VALUE);

    // end the SPI transaction and release the SPI bus
    this->endTransaction();

    this->wiper_value = AD5290_MIDPOINT_WIPER_VALUE;

    // use the first SPI transaction during init to check if communication is working
    if (lowByte(spi_received_value) != 0xAA) {
      // indicates that the recieved data did not match the first transmitted byte
      // if two AD5290s are daisy chained, or AD5290 CIPO is unused, this can be safely ignored
      return false;
    }

    // initialization was successful
    return true;
  }

  // set a signle potentiometer to a value between 0 [min] and 255 [max]
  void AD5290::set(uint8_t wiper_value) {
    // set digital potentiometer wiper to the specified value
    this->wiper_value = wiper_value;
    this->beginTransaction();
    SPI.transfer(this->wiper_value);
    this->endTransaction();
  }

  // initialize the SPI interface and take ownership of SPI bus
  void AD5290::beginTransaction(void) {
    SPI.beginTransaction(SPISettings(AD5290_SPI_SPEEDMAXIMUM, 
                                     AD5290_SPI_DATAORDER, 
                                     AD5290_SPI_DATAMODE));

    // assert the SPI chip select pin to begin SPI transaction
    digitalWrite(this->spi_chip_select, LOW);
  }

  // end the SPI transaction and release ownership of SPI bus
  void AD5290::endTransaction(void) {
    // de-assert the SPI chip select pin to end SPI transaction
    digitalWrite(this->spi_chip_select, HIGH);

    // release ownership of the SPI bus
    SPI.endTransaction();
  }
}

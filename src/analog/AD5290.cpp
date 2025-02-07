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
  AD5290::AD5290(uint8_t spi_chip_select, uint32_t spi_bus_speed) : 
    spi_chip_select(spi_chip_select), 
    spi_bus_speed((spi_bus_speed > AD5290_SPI_SPEEDMAXIMUM) ? AD5290_SPI_SPEEDMAXIMUM : spi_bus_speed),
    number_of_devices(AD5290_MINIMUM_DEVICE_COUNT) {
      this->wiper_data[this->number_of_devices] ={0U};
      pinMode(this->spi_chip_select, OUTPUT);
      digitalWrite(this->spi_chip_select, HIGH);
      SPI.begin();
  }
  AD5290::AD5290(uint8_t spi_chip_select, uint32_t spi_bus_speed, uint8_t number_of_devices) : 
    spi_chip_select(spi_chip_select), 
    spi_bus_speed((spi_bus_speed > AD5290_SPI_SPEEDMAXIMUM) ? AD5290_SPI_SPEEDMAXIMUM : spi_bus_speed),
    number_of_devices((number_of_devices > AD5290_MINIMUM_DEVICE_COUNT) ? 
                      ((number_of_devices < AD5290_MAXIMUM_DEVICE_COUNT) ? 
                        number_of_devices : AD5290_MAXIMUM_DEVICE_COUNT) : AD5290_MINIMUM_DEVICE_COUNT) {
      this->wiper_data[this->number_of_devices] ={0U};
      pinMode(this->spi_chip_select, OUTPUT);
      digitalWrite(this->spi_chip_select, HIGH);
      SPI.begin();
  }

  // initialize and configure the device
  bool AD5290::init(void) {
    memset(this->wiper_data, AD5290_MIDPOINT_WIPER_VALUE, this->number_of_devices);

    // configure SPI interface and begin the SPI transaction
    this->beginTransaction();

    // send readback value to ensure SPI is working, then set potentiometer value to midpoint value
    uint8_t spi_received_value = 0xAA;
    for (uint8_t idx = 0; idx < this->number_of_devices; idx++) {
      SPI.transfer(0xAA);
    }
    for (uint8_t idx = 0; idx < this->number_of_devices; idx++) {
      spi_received_value &= (uint8_t)SPI.transfer(this->wiper_data[idx]);
    }

    // end the SPI transaction and release the SPI bus
    this->endTransaction();

    // use the first SPI transaction during init to check if communication is working
    if (spi_received_value != 0xAA) {
      // indicates that the recieved data did not match the first transmitted byte
      // if two AD5290s are daisy chained, or AD5290 CIPO is unused, this can be safely ignored
      return false;
    }

    // initialization was successful
    return true;
  }

  // set a single potentiometer to a value between 0 [min] and 255 [max]
  void AD5290::set(uint8_t wiper_data) {
    // set digital potentiometer wiper to the specified value
    this->wiper_data[0] = wiper_data;
    this->beginTransaction();
    SPI.transfer(this->wiper_data[0]);
    this->endTransaction();
  }

  // set multiple potentiometers to a values between 0 [min] and 255 [max]
  bool AD5290::set(uint8_t* array_values, size_t array_size) {
    if (array_size != (size_t)this->number_of_devices) {
      return false;
    }

    // set an array of digital potentiometers to the specified value
    memcpy(this->wiper_data, &array_values, array_size);
    this->beginTransaction();
    for (uint8_t idx = 0; idx < this->number_of_devices; idx++) {
      SPI.transfer(this->wiper_data[idx]);
    }
    this->endTransaction();

    return true;
  }

  // read back the value of a single potentiometer
  uint8_t AD5290::get(void) {
    // get digital potentiometer wiper position
    this->beginTransaction();
    this->wiper_data[0] = (uint8_t)SPI.transfer(AD5290_MIDPOINT_WIPER_VALUE);
    SPI.transfer(this->wiper_data[0]);
    this->endTransaction();

    return this->wiper_data[0];
  }

  // read back the values of multiple potentiometers
  bool AD5290::get(uint8_t* array_values, size_t array_size) {
    if ((uint8_t)array_size != this->number_of_devices) {
      return false;
    }

    // get the full daisy-chain of digital potentiometer wiper positions
    this->beginTransaction();
    for (uint8_t idx = 0; idx < this->number_of_devices; idx++) {
      this->wiper_data[idx] = (uint8_t)SPI.transfer(AD5290_MIDPOINT_WIPER_VALUE);
    }
    for (uint8_t idx = 0; idx < this->number_of_devices; idx++) {
      (uint8_t)SPI.transfer(this->wiper_data[idx]);
    }
    this->endTransaction();
    
    memcpy(array_values, &this->wiper_data, array_size);

    return true;
  }

  // initialize the SPI interface and take ownership of SPI bus
  void AD5290::beginTransaction(void) {
    SPI.beginTransaction(SPISettings(this->spi_bus_speed, 
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

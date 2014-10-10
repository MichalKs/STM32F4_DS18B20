/**
 * @file: 	onewire.c
 * @brief:	   
 * @date: 	30 lip 2014
 * @author: Michal Ksiezopolski
 * 
 *
 * @details All the functions have been written based
 * on the Maxim Integrated DS18B20 datasheet.
 *
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <stm32f4xx.h>
#include <onewire.h>
#include <onewire_hal.h>
#include <timers.h>
#include <stdio.h>


#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf("1WIRE--> "str"%s",##args,"\r")
#define println(str, args...) printf("1WIRE--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif


#define ONEWIRE_MAX_DEVICES 16 ///< Maximum number of devices on the bus

//static uint64_t romCode[ONEWIRE_MAX_DEVICES]; ///< Romcodes of found devices.
//static uint16_t deviceCounter; ///< Number of found devices on the bus.

#define ONEWIRE_CMD_SEARCH_ROM    0xf0
#define ONEWIRE_CMD_READ_ROM      0x33
#define ONEWIRE_CMD_MATCH_ROM     0x55
#define ONEWIRE_CMD_SKIP_ROM      0xcc
#define ONEWIRE_CMD_ALARM_SEARCH  0xec


/**
 * @brief Initialize ONEWIRE bus.
 */
void ONEWIRE_Init(void) {

  // initialize hardware
  ONEWIRE_HAL_Init();

}

/**
 * @brief Reset the bus
 * @retval 0 Devices present on bus
 * @retval 1 No devices on bus
 */
uint8_t ONEWIRE_ResetBus(void) {

  ONEWIRE_HAL_BusLow(); // pull bus low for 480us
  TIMER_DelayUS(480);
  ONEWIRE_HAL_ReleaseBus(); // release bus for 60us
  TIMER_DelayUS(60);

  // by now device should pull bus low - presence pulse

  uint8_t ret = ONEWIRE_HAL_ReadBus();

  TIMER_DelayUS(420); // minimum 480us (after realase time) - 60us

  if (ret) {
    println("No devices");
  } else {
    println("Devices present on bus");
  }

  return ret;

}
/**
 * @brief Writes a bit
 * @param bit Bit
 */
void ONEWIRE_WriteBit(uint8_t bit) {
  ONEWIRE_HAL_BusLow(); // pull bus low for 1us
  TIMER_DelayUS(1);

  // release bus for high bit
  if (bit & 0x01) {
    ONEWIRE_HAL_ReleaseBus();
  }
  TIMER_DelayUS(60);
  ONEWIRE_HAL_ReleaseBus(); // this is necessary for 0 bit
  TIMER_DelayUS(1); // this delay is crucial - doesn't work without it
}
/**
 * @brief Writes a byte
 * @param data Byte
 */
void ONEWIRE_WriteByte(uint8_t data) {

  // data on ONEWIRE is sent LSB first

  for (uint8_t i = 0; i < 8; i++) {
    ONEWIRE_WriteBit(data & 0x01);
    data >>= 1; // shift data
  }

}
/**
 * @brief Reads a bit
 * @return Read bit
 */
uint8_t ONEWIRE_ReadBit(void) {

  ONEWIRE_HAL_BusLow(); // pull bus low for 1us
  TIMER_DelayUS(1);

  ONEWIRE_HAL_ReleaseBus();
  TIMER_DelayUS(15); // delay for device to respond - must be under 15us from initial falling edge

  uint8_t ret = ONEWIRE_HAL_ReadBus();

  TIMER_DelayUS(45); // whole read slot should be 60us + 1us of gap

  return ret;
}
/**
 * @brief Reads a byte
 * @return Read byte
 */
uint8_t ONEWIRE_ReadByte(void) {

  uint8_t ret = 0;

  for (uint8_t i = 0; i < 8; i++) {
    ret |= (ONEWIRE_ReadBit() << i);
  }

  return ret;
}

/**
 * @brief Reads ROM code of device on the bus
 *
 * @warning This command works only if there is only one device
 * on the bus. Data collision will occur if there are more than
 * one device.
 *
 * @param buf Buffer for storing ROM code
 */
uint8_t ONEWIRE_ReadROM(uint8_t* buf) {

  uint8_t ret = ONEWIRE_ResetBus();

  if (ret) {
    return 1; // no devices on bus
  }

  ONEWIRE_WriteByte(ONEWIRE_CMD_READ_ROM); // read ROM

  for (int i = 0; i < 8; i++) {
    buf[i] = ONEWIRE_ReadByte();
    print("0x%02x ", buf[i]);
  }
  println("\r\n");

  return 0;

}
/**
 * @brief Send match ROM command
 * @param rom ROM code
 */
void ONEWIRE_MatchROM(uint8_t* rom) {

  ONEWIRE_ResetBus();
  ONEWIRE_WriteByte(ONEWIRE_CMD_MATCH_ROM); // match ROM
  for (int i = 0; i < 8; i++) {
    ONEWIRE_WriteByte(rom[i]);
  }

}



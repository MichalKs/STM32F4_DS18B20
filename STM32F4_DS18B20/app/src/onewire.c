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


#define ONEWIRE_MAX_DEVICES 16 ///< Maximum number of devices on the bus

static uint64_t romCode[ONEWIRE_MAX_DEVICES]; ///< Romcodes of found devices.
static uint16_t deviceCounter; ///< Number of found devices on the bus.

#define ONEWIRE_CMD_SEARCH_ROM    0xf0
#define ONEWIRE_CMD_READ_ROM      0x33
#define ONEWIRE_CMD_MATCH_ROM     0x55
#define ONEWIRE_CMD_SKIP_ROM      0xcc
#define ONEWIRE_CMD_ALARM_SEARCH  0xec


/**
 * @brief Initialize onewire bus.
 */
void ONEWIRE_Init(void) {

  // initialize hardware
  ONEWIRE_HAL_Init();

}

/**
 * @brief Reset the bus
 */
void ONEWIRE_ResetBus(void) {

  // pull bus low for 480us
  ONEWIRE_HAL_BusLow();

}



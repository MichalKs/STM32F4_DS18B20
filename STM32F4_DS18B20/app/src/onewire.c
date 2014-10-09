/**
 * @file: 	onewire.c
 * @brief:	   
 * @date: 	30 lip 2014
 * @author: Michal Ksiezopolski
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

#define ONEWIRE_PIN GPIO_Pin_1
#define ONEWIRE_PORT GPIOC
#define ONEWIRE_CLK RCC_AHB1Periph_GPIOC

#define ONEWIRE_MAX_DEVICES 16 ///< Maximum number of devices on the bus

static uint64_t romCode[ONEWIRE_MAX_DEVICES]; ///< Romcodes of found devices.
static uint16_t deviceCounter; ///< Number of found devices on the bus.

#define ONEWIRE_CMD_SEARCH_ROM  0xf0
#define ONEWIRE_CMD_READ_ROM    0x33
#define ONEWIRE_CMD_MATCH_ROM   0x55
#define ONEWIRE_CMD_SKIP_ROM    0xcc
#define ONEWIRE_CMD_ALARM_SEARCH  0xec


/**
 * @brief Initialize onewire pin in open drain output mode.
 */
void ONEWIRE_Init(void) {

  RCC_AHB1PeriphClockCmd(ONEWIRE_CLK, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output open drain mode
  GPIO_InitStructure.GPIO_Pin   = ONEWIRE_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(ONEWIRE_PORT, &GPIO_InitStructure);

}

void ONEWIRE_ResetBus(void) {

}



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

#ifndef ONEWIRE_C_
#define ONEWIRE_C_

#include <inttypes.h>

uint8_t ONEWIRE_ResetBus  (void);
uint8_t ONEWIRE_ReadByte  (void);
void    ONEWIRE_WriteByte (uint8_t data);
void    ONEWIRE_Init      (void);
uint8_t ONEWIRE_ReadROM   (uint8_t* buf);
void    ONEWIRE_MatchROM  (uint8_t* rom);

#endif /* ONEWIRE_C_ */

/**
 * @file: 	ds18b20.c
 * @brief:	   
 * @date: 	5 sie 2014
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


#include <ds18b20.h>
#include <onewire.h>

#include <inttypes.h>

typedef struct {
  uint8_t tempLSB;
  uint8_t tempMSB;
  uint8_t thReg;
  uint8_t tlReg;
  uint8_t config;
  uint8_t reserved0;
  uint8_t reserved1;
  uint8_t reserved2;
  uint8_t crc;
} __attribute((packed)) DS18B20_Memory;

#define DS18B20_RESOLUTION9   (0<<5)
#define DS18B20_RESOLUTION10  (1<<5)
#define DS18B20_RESOLUTION11  (2<<5)
#define DS18B20_RESOLUTION12  (3<<5)



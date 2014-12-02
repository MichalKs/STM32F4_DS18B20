/**
 * @file: 	ds18b20.c
 * @brief:	DS18B20 digital thermometer library
 * @date: 	5 sie 2014
 * @author: Michal Ksiezopolski
 * 
 * @details The DS18B20 is assumed to be in non-parasite mode
 * (TODO Check if parasite mode also works with library).
 * The DS18B20 data line should be pulled up with a 4k7 resistor.
 *
 * TODO Add power supply read, recall EEPROM, use of multiple DS18B20.
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

#include <stdio.h>

#define DEBUG ///< If defined, program sends all printf data to the USART

#ifdef DEBUG
#define print(str, args...) printf("DS18B20--> "str"%s",##args,"\r")
#define println(str, args...) printf("DS18B20--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif

#define DS18B20_CMD_CONVERT_T         0x44 ///< Convert temperature command
#define DS18B20_CMD_WRITE_SCRATCHPAD  0x4e ///< Write scratchpad command
#define DS18B20_CMD_READ_SCRATCHPAD   0xbe ///< Read scratchpad command
#define DS18B20_CMD_COPY_SCRATCHPAD   0x48 ///< Copy scratchpad to internal EEPROM command
#define DS18B20_CMD_RECALL_EE         0xb8 ///< Recall configuration data from internal EEPROM command
#define DS18B20_CMD_READ_POWER        0xb4 ///< Read power status command

/**
 * @brief Memory structure of the DS18B20
 */
typedef struct {
  uint8_t tempLSB;    ///< Temperature value - lower byte
  uint8_t tempMSB;    ///< Temperature value - higher byte
  uint8_t thReg;      ///< Temperature alarm - higher byte
  uint8_t tlReg;      ///< Temperature alarm - lower byte
  uint8_t config;     ///< Configuration byte
  uint8_t reserved0;  ///< Reserved
  uint8_t reserved1;  ///< Reserved
  uint8_t reserved2;  ///< Reserved
  uint8_t crc;        ///< CRC calculated from previous fields
} __attribute((packed)) DS18B20_Memory;

#define DS18B20_RESOLUTION9   (0<<5) ///< 9  bit resolution
#define DS18B20_RESOLUTION10  (1<<5) ///< 10 bit resolution
#define DS18B20_RESOLUTION11  (2<<5) ///< 11 bit resolution
#define DS18B20_RESOLUTION12  (3<<5) ///< 12 bit resolution (default)

static uint8_t romCode[8]; ///< Device ROMCODE

#define ROMCODE_DEV_ID 0x28 ///< Device ROMCODE ID for DS18B20 family

/**
 * @brief Initialize DS18B20 digital thermometer.
 *
 * @retval 1 No DS18B20 on bus
 * @retval 0 Initialization went OK.
 */
uint8_t DS18B20_Init(void) {

  ONEWIRE_ReadROM(romCode);

  if (romCode[0] != ROMCODE_DEV_ID) {
    println("Not DS18B20!");
    return 1; // not DS18B20
  }

  return 0;

}
/**
 * @brief Send start temperature conversion command.
 */
void DS18B20_ConversionStart(void) {

  ONEWIRE_MatchROM(romCode);

  ONEWIRE_WriteByte(DS18B20_CMD_CONVERT_T); // convert temp

}
/**
 * @brief Write scratchpad commands
 * @param th High byte of temperature alarm value
 * @param tl Low byte of temperature alarm value
 * @param conf Configuration byte
 */
void DS18B20_WriteScratchPad(uint8_t th, uint8_t tl, uint8_t conf) {

  ONEWIRE_MatchROM(romCode);
  ONEWIRE_WriteByte(DS18B20_CMD_WRITE_SCRATCHPAD);

  ONEWIRE_WriteByte(th); // high alarm temperature
  ONEWIRE_WriteByte(tl); // low  alarm temperature

  conf |= 0x1f; // 5 LSB bits always 1
  conf &= ~(1<<7); // MSB always 0
  ONEWIRE_WriteByte(conf);

}
/**
 * @brief Copies scratchpad into DS18B20 EEPROM.
 * Configuration will be restored after powerdown.
 */
void DS18B20_CopyScratchPad(void) {

  ONEWIRE_MatchROM(romCode);
  ONEWIRE_WriteByte(DS18B20_CMD_COPY_SCRATCHPAD);

}
/**
 * @brief Reads DS18B20 scratchpad.
 * @param buf Buffer for scratchpad
 */
void DS18B20_ReadScratchPad(uint8_t* buf) {

  ONEWIRE_MatchROM(romCode);

  ONEWIRE_WriteByte(DS18B20_CMD_READ_SCRATCHPAD); // read scratchpad

  for (int i = 0; i < 9; i++) {
    buf[i] = ONEWIRE_ReadByte();
  }

}
/**
 * @brief Reads DS18B20 temperature.
 *
 * @return Temperature value in degrees Celsius
 */
double DS18B20_ReadTemp(void) {

  uint8_t mem[10];

  DS18B20_ReadScratchPad(mem);

  DS18B20_Memory* dsMem = (DS18B20_Memory*)mem;

  uint8_t t1 = (dsMem->tempLSB >> 4) & 0x0f;

  t1 |= ((dsMem->tempMSB << 4) & 0x70);

  double t2 = 0;

  if (dsMem->tempLSB & 0x08) {
    t2 += 0.5;
  }

  if (dsMem->tempLSB & 0x04) {
    t2 += 0.25;
  }

  if (dsMem->tempLSB & 0x02) {
    t2 += 0.125;
  }

  if (dsMem->tempLSB & 0x01) {
    t2 += 0.0625;
  }

  double ret = (double)t1 + t2;

  return ret;

}

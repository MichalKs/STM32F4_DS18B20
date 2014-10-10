/**
 * @file: 	main.c
 * @brief:	LED test
 * @date: 	9 kwi 2014
 * @author: Michal Ksiezopolski
 *
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

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <timers.h>
#include <led.h>
#include <comm.h>
#include <keys.h>
#include <onewire.h>

#define SYSTICK_FREQ 1000 ///< Frequency of the SysTick set at 1kHz.
#define COMM_BAUD_RATE 115200UL ///< Baud rate for communication with PC

void softTimerCallback(void);

#define DEBUG

#ifdef DEBUG
#define print(str, args...) printf("MAIN--> "str"%s",##args,"\r")
#define println(str, args...) printf("MAIN--> "str"%s",##args,"\r\n")
#else
#define print(str, args...) (void)0
#define println(str, args...) (void)0
#endif


int main(void) {
	
  COMM_Init(COMM_BAUD_RATE);
  println("Starting program"); // Print a string to terminal

	TIMER_Init(SYSTICK_FREQ); // Initialize timer

	// Add a soft timer with callback running every 1000ms
	int8_t timerID = TIMER_AddSoftTimer(1000, softTimerCallback);
	TIMER_StartSoftTimer(timerID);

	LED_Init(LED0); // Add an LED
	LED_Init(LED1); // Add an LED
	LED_Init(LED2); // Add an LED
	LED_Init(LED3); // Add an LED
	LED_Init(LED5); // Add nonexising LED for test
	LED_ChangeState(LED5, LED_ON);

	KEYS_Init(); // initialize matrix keyboard

  uint8_t buf[255];
  uint8_t len;

  uint32_t softTimer = TIMER_GetTime(); // get start time for delay

  ONEWIRE_Init();
  ONEWIRE_ResetBus();
  ONEWIRE_WriteByte(0x33); // read ROM
  uint8_t rom[8];

  for (int i = 0; i < 8; i++) {
    rom[i] = ONEWIRE_ReadByte();
    printf("0x%02x ", rom[i]);
  }
  printf("\r\n");

  TIMER_DelayUS(1000);
  ONEWIRE_ResetBus();
  ONEWIRE_WriteByte(0x55); // match ROM
  for (int i = 0; i < 8; i++) {
    ONEWIRE_WriteByte(rom[i]);
  }
  ONEWIRE_WriteByte(0x44); // convert temp

  TIMER_Delay(1000); // wait 1s for temperature
  ONEWIRE_ResetBus();
  ONEWIRE_WriteByte(0x55); // match ROM
  for (int i = 0; i < 8; i++) {
    ONEWIRE_WriteByte(rom[i]);
  }
  ONEWIRE_WriteByte(0xbe); // read scratchpad

  uint8_t result[10];
  for (int i = 0; i < 9; i++) {
    result[i] = ONEWIRE_ReadByte();
    printf("0x%02x ", result[i]);
  }
  printf("\r\n");

	while (1) {

	  if (TIMER_DelayTimer(1000, softTimer)) {
	    LED_Toggle(LED3);
	    softTimer = TIMER_GetTime(); // get start time for delay
	  }

	  // check for new frames from PC
	  if (!COMM_GetFrame(buf, &len)) {
	    println("Got frame of length %d: %s", (int)len, (char*)buf);

	    // control LED0 from terminal
	    if (!strcmp((char*)buf, ":LED0 ON")) {
	      LED_ChangeState(LED0, LED_ON);
	    }
	    if (!strcmp((char*)buf, ":LED0 OFF")) {
	      LED_ChangeState(LED0, LED_OFF);
	    }
	  }

		TIMER_SoftTimersUpdate(); // run timers
		KEYS_Update(); // run keyboard
	}
}
/**
 * @brief Callback function called on every soft timer overflow
 */
void softTimerCallback(void) {

//  uint32_t count = TIMER14_GetCount();
//
//  println("Timer14: %u", count);

  LED_Toggle(LED0); // Toggle LED
  //printf("Test string sent from STM32F4!!!\r\n"); // Print test string

}

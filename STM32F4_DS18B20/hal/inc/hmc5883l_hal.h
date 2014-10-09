/**
 * @file: 	hmc5883l_hal.h
 * @brief:	   
 * @date: 	9 paź 2014
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

#ifndef HMC5883L_HAL_H_
#define HMC5883L_HAL_H_

void HMC5883L_HAL_Init(void);
uint8_t HMC5883L_HAL_Read(uint8_t address);
void HMC5883L_HAL_Write(uint8_t address, uint8_t data);

#endif /* HMC5883L_HAL_H_ */

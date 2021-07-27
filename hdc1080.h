/*
	Driver for the hdc1080 sensor for stm32.
	Copyright (C) 2019  Paulo Pereira	(EEIC-UMinho)
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file hdc1080.h
 * @brief Driver for the hdc1080 sensor for stm32.
 */

#ifndef _HDC1080_H_
#define _HDC1080_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stdint.h"
	
	#define HDC1080_I2C_ADDR	0x40
	#define HDC1080_I2C_HANDLER	hi2c2
	
	
	//I2C REGISTERS
	#define TEMPERATURE		0x00 
	#define	HUMIDITY		0x01
	#define	CONFIGURATION	0x02
	#define	SERIAL_ID		0xFB
	#define	MANUFACTURER_ID	0xFE
	#define	DEVICE_ID		0xFF
	
	//CONFIGURATION Register
	#define	RST			0x0F	//self clearing reset
	#define	HEAT		0x0D	//heater
	#define	MODE		0x0C
	#define	BTST		0x0B
	#define	TRES(X)		(( X&0x1 )<<10)
	#define	TRES_14BIT	0x0
	#define	TRES_11BIT	0x1
	#define	HRES(X)		(( X&0x3 )<<8)
	#define	HRES_14BIT	0x0
	#define	HRES_11BIT	0x1
	#define	HRES_8BIT	0x2
	
	
	void	hdc1080_init(void);
	float	hdc1080_measureT(void);
	float	hdc1080_measureRH(void);
	void	hdc1080_measure_T_RH(float*, float*);

#ifdef __cplusplus
}
#endif
	
#endif /* _HDC1080_H_ */

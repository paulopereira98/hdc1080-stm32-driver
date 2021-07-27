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
 * @file hdc1080.c
 * @brief Driver for the hdc1080 sensor for stm32.
 */

#include "hdc1080.h"
#include "stdint.h"
#include "i2c.h"
#include "cmsis_os.h"
#include "stdio.h"

/**
  * @brief  initialization process for the hdc1080 sensor
  * @retval None
  */
void hdc1080_init(void)
{
	uint16_t	configuration = 0;
	uint8_t		config_arr[2];
	
	configuration = ( RST | MODE | TRES(TRES_14BIT) | HRES(HRES_14BIT) );
	
	config_arr[0] = (configuration & 0xFF00)>>8;	//configuration MSB
	config_arr[1] = (configuration & 0x00FF);	//configuration LSB
	
	HAL_I2C_Mem_Write(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, CONFIGURATION, I2C_MEMADD_SIZE_8BIT, config_arr, 2, 100);
}

/**
  * @brief  read sensor i2c temperature register
  * @retval temperature in celcius
  */
float hdc1080_measureT(void)
{
	uint8_t receive_data[2];
	uint32_t t;
	uint8_t send_data = TEMPERATURE;
	
	//trigger reading
	HAL_I2C_Master_Transmit(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, &send_data, 1, 1000);
	
	//conversion takes 6.50ms(temperature)
	osDelay(10);

	//Read temperature and humidity
	if(HAL_I2C_Master_Receive(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, receive_data, 2, 2000)== HAL_ERROR)
	{
		osDelay(5);
		HAL_I2C_Master_Receive(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, receive_data, 2, 2000);
	}
		
	t =  ( (receive_data[0]<<8) | receive_data[1] );

	return (float)( ((t/65536.0)*165.0)-40.0 );
}

/**
  * @brief  read sensor i2c humidity register
  * @retval relative humidity in percentage
  */
float hdc1080_measureRH(void)
{
	uint8_t receive_data[2];
	uint32_t rh;
	uint8_t send_data = HUMIDITY;//temperature and humidity
	
	//trigger reading
	HAL_I2C_Master_Transmit(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, &send_data, 1, 1000);
	
	//conversion takes 6.35ms(humidity)
	osDelay(10);

	//Read temperature and humidity
	if(HAL_I2C_Master_Receive(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, receive_data, 2, 2000)== HAL_ERROR)
	{
		osDelay(5);
		HAL_I2C_Master_Receive(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, receive_data, 2, 2000);
	}
		
	rh = ( (receive_data[0]<<8) | receive_data[1] );

	return (float)( (rh/65536.0)*100.0 );
}

/**
  * @brief  read sensor i2c temperature and humidity registers
  * @param  temperature		pointer to temperature variable
  * @param  humidity 		pointer to humidity variable
  * @retval None
  */
void hdc1080_measure_T_RH(float *temperature, float *humidity)
{

	*temperature = hdc1080_measureT();
	*humidity =    hdc1080_measureRH();
}

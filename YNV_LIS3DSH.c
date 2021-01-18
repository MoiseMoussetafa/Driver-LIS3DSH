/*
 * YNV_LIS3DSH.c
 *
 *  Created on: Jan 17, 2021
 *      Author: Moise
 */

#include "YNV_LIS3DSH.h"

/* Write register */
LIS3DSH_Status LIS3DSH_Write_reg(SPI_HandleTypeDef *hspi,
		uint8_t reg_addr,
		uint8_t *dataW,
		uint8_t size)
{
	reg_addr &= 0x7F; //Masking : 1st bit at 0 for writing
	uint8_t tab[2] = {reg_addr, *dataW};

	CS_LOW;
	if(HAL_SPI_Transmit(hspi, tab, 2, 10) == HAL_OK)
	{
		CS_HIGH;
		return LIS3DSH_OK;
	}
	CS_HIGH;
	return LIS3DSH_ERROR;
}

/* Read register */
LIS3DSH_Status LIS3DSH_Read_reg(SPI_HandleTypeDef *hspi,
		uint8_t reg_addr,
		uint8_t *dataR,
		uint8_t size)
{
	reg_addr |= 0x80; //Masking : 1st bit at 1 for reading
	CS_LOW;
	if(HAL_SPI_Transmit(hspi, &reg_addr, 1, 10) == HAL_OK)
	{
		if(HAL_SPI_Receive(hspi, dataR, size, 10) == HAL_OK)
		{
			CS_HIGH;
			return LIS3DSH_OK;
		}
	}
	CS_HIGH;
	return LIS3DSH_ERROR;
}

/* Init LIS3DSH */
LIS3DSH_Status LIS3DSH_Init_t(SPI_HandleTypeDef *hspi,
		LIS3DSH_Init *posInitDef)
{
	uint8_t spiData[2] = {0x00, 0x00};		//Data to send to LIS3DSH
	uint8_t spiCheckData[2] = {0x00, 0x00};	//Data to read from LISH3DSH

	//Config REG4
	spiData[0] |= (posInitDef->power | LIS3DSH_ON);
	spiData[0] |= (posInitDef->axe | LIS3DSH_XY);

	//Config REG5
	spiData[1] |= (posInitDef->scale | LIS3DSH_SCALE_4G);

	if(LIS3DSH_Write_reg(hspi, CTRL_REG4, &spiData[0], 1) == LIS3DSH_OK)
	{
		if(LIS3DSH_Read_reg(hspi, CTRL_REG4, &spiCheckData[0], 1) == LIS3DSH_OK)
		{
			if(spiCheckData[0] == spiData[0])
			{
				if(LIS3DSH_Write_reg(hspi, CTRL_REG5, &spiData[1], 1) == LIS3DSH_OK)
				{
					if(LIS3DSH_Read_reg(hspi, CTRL_REG5, &spiCheckData[1], 1) == LIS3DSH_OK)
					{
						if(spiCheckData[1] == spiData[1])
						{
							return LIS3DSH_OK;
						}
					}
				}
			}
		}
	}
	return LIS3DSH_ERROR;
}

/* Get position */
LIS3DSH_Status LIS3DSH_Get_Pos(SPI_HandleTypeDef *hspi,
		LIS3DSH_Result* structResult)
{
	uint8_t posXL = 0x00;
	uint8_t posXH = 0x00;
	uint8_t posYL = 0x00;
	uint8_t posYH = 0x00;

	if(LIS3DSH_Read_reg(hspi, OUT_X_L, &posXL, 1) == LIS3DSH_OK)
	{
		if(LIS3DSH_Read_reg(hspi, OUT_X_H, &posXH, 1) == LIS3DSH_OK)
		{
			structResult->resultX = (posXH << 8 | posXL);

			if(LIS3DSH_Read_reg(hspi, OUT_Y_L, &posYL, 1) == LIS3DSH_OK)
			{
				if(LIS3DSH_Read_reg(hspi, OUT_Y_H, &posYH, 1) == LIS3DSH_OK)
				{
					structResult->resultY = (posYH << 8 | posYL);
					return LIS3DSH_OK;
				}
			}
		}
	}
	return LIS3DSH_ERROR;
}
